//
// Copyright (C) Microsoft Corporation. All rights reserved.
//

#pragma once

DECLARE_HANDLE(XDP_PROVIDER_HANDLE);

typedef
VOID
XDP_PROVIDER_DETACH_HANDLER(
    _In_ VOID *ProviderContext
    );

VOID
XdpCloseProvider(
    _In_ XDP_PROVIDER_HANDLE ProviderHandle
    );

VOID
XdpCleanupProvider(
    _In_ XDP_PROVIDER_HANDLE ProviderHandle
    );

NTSTATUS
XdpOpenProvider(
    _In_ UINT32 ApiVersion,
    _In_ UINT32 InterfaceIndex,
    _In_ CONST GUID *ClientGuid,
    _In_ VOID *ProviderContext,
    _In_ XDP_PROVIDER_DETACH_HANDLER *DetachHandler,
    _Out_ VOID **InterfaceContext,
    _Out_ CONST XDP_INTERFACE_DISPATCH **InterfaceDispatch,
    _Out_ XDP_PROVIDER_HANDLE *ProviderHandle
    );