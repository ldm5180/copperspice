/***********************************************************************
*
* Copyright (c) 2012-2018 Barbara Geller
* Copyright (c) 2012-2018 Ansel Sermersheim
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#include <qnetworkinterface.h>
#include <qnetworkinterface_p.h>

#ifndef QT_NO_NETWORKINTERFACE

#include <qnetworkinterface_win_p.h>
#include <qhostinfo.h>
#include <qhash.h>
#include <qurl.h>
#include <qsystemlibrary_p.h>

QT_BEGIN_NAMESPACE

typedef DWORD (WINAPI *PtrGetAdaptersInfo)(PIP_ADAPTER_INFO, PULONG);
static PtrGetAdaptersInfo ptrGetAdaptersInfo = 0;
typedef ULONG (WINAPI *PtrGetAdaptersAddresses)(ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG);
static PtrGetAdaptersAddresses ptrGetAdaptersAddresses = 0;
typedef DWORD (WINAPI *PtrGetNetworkParams)(PFIXED_INFO, PULONG);
static PtrGetNetworkParams ptrGetNetworkParams = 0;

static void resolveLibs()
{
   // try to find the functions we need from Iphlpapi.dll
   static bool done = false;
   if (!done) {
      QSystemLibrary iphlpapi(QLatin1String("iphlpapi"));
      if (iphlpapi.load()) {
         ptrGetAdaptersInfo = (PtrGetAdaptersInfo)iphlpapi.resolve("GetAdaptersInfo");
         ptrGetAdaptersAddresses = (PtrGetAdaptersAddresses)iphlpapi.resolve("GetAdaptersAddresses");
         ptrGetNetworkParams = (PtrGetNetworkParams)iphlpapi.resolve("GetNetworkParams");
      }
      done = true;
   }
}

static QHostAddress addressFromSockaddr(sockaddr *sa)
{
   QHostAddress address;
   if (!sa) {
      return address;
   }

   if (sa->sa_family == AF_INET) {
      address.setAddress(htonl(((sockaddr_in *)sa)->sin_addr.s_addr));
   } else if (sa->sa_family == AF_INET6) {
      address.setAddress(((qt_sockaddr_in6 *)sa)->sin6_addr.qt_s6_addr);
      int scope = ((qt_sockaddr_in6 *)sa)->sin6_scope_id;
      if (scope) {
         address.setScopeId(QString::number(scope));
      }
   } else {
      qWarning("Got unknown socket family %d", sa->sa_family);
   }
   return address;

}

static QHash<QHostAddress, QHostAddress> ipv4Netmasks()
{
   //Retrieve all the IPV4 addresses & netmasks
   IP_ADAPTER_INFO staticBuf[2]; // 2 is arbitrary
   PIP_ADAPTER_INFO pAdapter = staticBuf;
   ULONG bufSize = sizeof staticBuf;
   QHash<QHostAddress, QHostAddress> ipv4netmasks;

   DWORD retval = ptrGetAdaptersInfo(pAdapter, &bufSize);
   if (retval == ERROR_BUFFER_OVERFLOW) {
      // need more memory
      pAdapter = (IP_ADAPTER_INFO *)malloc(bufSize);
      if (!pAdapter) {
         return ipv4netmasks;
      }
      // try again
      if (ptrGetAdaptersInfo(pAdapter, &bufSize) != ERROR_SUCCESS) {
         free(pAdapter);
         return ipv4netmasks;
      }
   } else if (retval != ERROR_SUCCESS) {
      // error
      return ipv4netmasks;
   }

   // iterate over the list and add the entries to our listing
   for (PIP_ADAPTER_INFO ptr = pAdapter; ptr; ptr = ptr->Next) {
      for (PIP_ADDR_STRING addr = &ptr->IpAddressList; addr; addr = addr->Next) {
         QHostAddress address(QLatin1String(addr->IpAddress.String));
         QHostAddress mask(QLatin1String(addr->IpMask.String));
         ipv4netmasks[address] = mask;
      }
   }
   if (pAdapter != staticBuf) {
      free(pAdapter);
   }

   return ipv4netmasks;

}

static QList<QNetworkInterfacePrivate *> interfaceListingWinXP()
{
   QList<QNetworkInterfacePrivate *> interfaces;
   IP_ADAPTER_ADDRESSES staticBuf[2]; // 2 is arbitrary
   PIP_ADAPTER_ADDRESSES pAdapter = staticBuf;
   ULONG bufSize = sizeof staticBuf;

   const QHash<QHostAddress, QHostAddress> &ipv4netmasks = ipv4Netmasks();
   ULONG flags = GAA_FLAG_INCLUDE_PREFIX |
                 GAA_FLAG_SKIP_DNS_SERVER |
                 GAA_FLAG_SKIP_MULTICAST;
   ULONG retval = ptrGetAdaptersAddresses(AF_UNSPEC, flags, NULL, pAdapter, &bufSize);
   if (retval == ERROR_BUFFER_OVERFLOW) {
      // need more memory
      pAdapter = (IP_ADAPTER_ADDRESSES *)malloc(bufSize);
      if (!pAdapter) {
         return interfaces;
      }
      // try again
      if (ptrGetAdaptersAddresses(AF_UNSPEC, flags, NULL, pAdapter, &bufSize) != ERROR_SUCCESS) {
         free(pAdapter);
         return interfaces;
      }
   } else if (retval != ERROR_SUCCESS) {
      // error
      return interfaces;
   }

   // iterate over the list and add the entries to our listing
   for (PIP_ADAPTER_ADDRESSES ptr = pAdapter; ptr; ptr = ptr->Next) {
      QNetworkInterfacePrivate *iface = new QNetworkInterfacePrivate;
      interfaces << iface;

      iface->index = 0;
      if (ptr->Length >= offsetof(IP_ADAPTER_ADDRESSES, Ipv6IfIndex) && ptr->Ipv6IfIndex != 0) {
         iface->index = ptr->Ipv6IfIndex;
      } else if (ptr->IfIndex != 0) {
         iface->index = ptr->IfIndex;
      }

      iface->flags = QNetworkInterface::CanBroadcast;
      if (ptr->OperStatus == IfOperStatusUp) {
         iface->flags |= QNetworkInterface::IsUp | QNetworkInterface::IsRunning;
      }
      if ((ptr->Flags & IP_ADAPTER_NO_MULTICAST) == 0) {
         iface->flags |= QNetworkInterface::CanMulticast;
      }

      iface->name = QString::fromLocal8Bit(ptr->AdapterName);
      iface->friendlyName = QString::fromWCharArray(ptr->FriendlyName);
      if (ptr->PhysicalAddressLength)
         iface->hardwareAddress = iface->makeHwAddress(ptr->PhysicalAddressLength,
                                  ptr->PhysicalAddress);
      else
         // loopback if it has no address
      {
         iface->flags |= QNetworkInterface::IsLoopBack;
      }

      // The GetAdaptersAddresses call has an interesting semantic:
      // It can return a number N of addresses and a number M of prefixes.
      // But if you have IPv6 addresses, generally N > M.
      // I cannot find a way to relate the Address to the Prefix, aside from stopping
      // the iteration at the last Prefix entry and assume that it applies to all addresses
      // from that point on.
      PIP_ADAPTER_PREFIX pprefix = 0;
      if (ptr->Length >= offsetof(IP_ADAPTER_ADDRESSES, FirstPrefix)) {
         pprefix = ptr->FirstPrefix;
      }
      for (PIP_ADAPTER_UNICAST_ADDRESS addr = ptr->FirstUnicastAddress; addr; addr = addr->Next) {
         QNetworkAddressEntry entry;
         entry.setIp(addressFromSockaddr(addr->Address.lpSockaddr));
         if (pprefix) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
               entry.setNetmask(ipv4netmasks[entry.ip()]);

               // broadcast address is set on postProcess()
            } else { //IPV6
               entry.setPrefixLength(pprefix->PrefixLength);
            }
            pprefix = pprefix->Next ? pprefix->Next : pprefix;
         }
         iface->addressEntries << entry;
      }
   }

   if (pAdapter != staticBuf) {
      free(pAdapter);
   }

   return interfaces;
}

static QList<QNetworkInterfacePrivate *> interfaceListingWin2k()
{
   QList<QNetworkInterfacePrivate *> interfaces;
   IP_ADAPTER_INFO staticBuf[2]; // 2 is arbitrary
   PIP_ADAPTER_INFO pAdapter = staticBuf;
   ULONG bufSize = sizeof staticBuf;

   DWORD retval = ptrGetAdaptersInfo(pAdapter, &bufSize);
   if (retval == ERROR_BUFFER_OVERFLOW) {
      // need more memory
      pAdapter = (IP_ADAPTER_INFO *)malloc(bufSize);
      if (!pAdapter) {
         return interfaces;
      }
      // try again
      if (ptrGetAdaptersInfo(pAdapter, &bufSize) != ERROR_SUCCESS) {
         free(pAdapter);
         return interfaces;
      }
   } else if (retval != ERROR_SUCCESS) {
      // error
      return interfaces;
   }

   // iterate over the list and add the entries to our listing
   for (PIP_ADAPTER_INFO ptr = pAdapter; ptr; ptr = ptr->Next) {
      QNetworkInterfacePrivate *iface = new QNetworkInterfacePrivate;
      interfaces << iface;

      iface->index = ptr->Index;
      iface->flags = QNetworkInterface::IsUp | QNetworkInterface::IsRunning;
      if (ptr->Type == MIB_IF_TYPE_PPP) {
         iface->flags |= QNetworkInterface::IsPointToPoint;
      } else {
         iface->flags |= QNetworkInterface::CanBroadcast;
      }
      iface->name = QString::fromLocal8Bit(ptr->AdapterName);
      iface->hardwareAddress = QNetworkInterfacePrivate::makeHwAddress(ptr->AddressLength,
                               ptr->Address);

      for (PIP_ADDR_STRING addr = &ptr->IpAddressList; addr; addr = addr->Next) {
         QNetworkAddressEntry entry;
         entry.setIp(QHostAddress(QLatin1String(addr->IpAddress.String)));
         entry.setNetmask(QHostAddress(QLatin1String(addr->IpMask.String)));
         // broadcast address is set on postProcess()

         iface->addressEntries << entry;
      }
   }

   if (pAdapter != staticBuf) {
      free(pAdapter);
   }

   return interfaces;
}

static QList<QNetworkInterfacePrivate *> interfaceListing()
{
   resolveLibs();
   if (ptrGetAdaptersAddresses != NULL) {
      return interfaceListingWinXP();
   } else if (ptrGetAdaptersInfo != NULL) {
      return interfaceListingWin2k();
   }

   // failed
   return QList<QNetworkInterfacePrivate *>();
}

QList<QNetworkInterfacePrivate *> QNetworkInterfaceManager::scan()
{
   return interfaceListing();
}

QString QHostInfo::localDomainName()
{
   resolveLibs();
   if (ptrGetNetworkParams == NULL) {
      return QString();   // couldn't resolve
   }

   FIXED_INFO info, *pinfo;
   ULONG bufSize = sizeof info;
   pinfo = &info;
   if (ptrGetNetworkParams(pinfo, &bufSize) == ERROR_BUFFER_OVERFLOW) {
      pinfo = (FIXED_INFO *)malloc(bufSize);
      if (!pinfo) {
         return QString();
      }
      // try again
      if (ptrGetNetworkParams(pinfo, &bufSize) != ERROR_SUCCESS) {
         free(pinfo);
         return QString();   // error
      }
   }

   QString domainName = QUrl::fromAce(pinfo->DomainName);

   if (pinfo != &info) {
      free(pinfo);
   }

   return domainName;
}

QT_END_NAMESPACE

#endif // QT_NO_NETWORKINTERFACE
