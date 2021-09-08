//
// Copyright (C) Microsoft Corporation. All rights reserved.
//

#pragma once

//
// Forward definitions
//
typedef struct _XDP_BINDING_HANDLE *XDP_BINDING_HANDLE;
typedef struct _XDP_BINDING_CLIENT_ENTRY XDP_BINDING_CLIENT_ENTRY;
typedef struct _XDP_BINDING_WORKITEM XDP_BINDING_WORKITEM;

//
// Serialized work queue callback.
//
typedef
VOID
XDP_BINDING_WORK_ROUTINE(
    _In_ XDP_BINDING_WORKITEM *Item
    );

//
// Element of the per-binding serialized work queue.
//
typedef struct _XDP_BINDING_WORKITEM {
    SINGLE_LIST_ENTRY Link;
    XDP_BINDING_HANDLE BindingHandle;
    XDP_BINDING_WORK_ROUTINE *WorkRoutine;
    PROCESSOR_NUMBER IdealProcessor;
} XDP_BINDING_WORKITEM;

_IRQL_requires_(PASSIVE_LEVEL)
XDP_BINDING_HANDLE
XdpIfFindAndReferenceBinding(
    _In_ NET_IFINDEX IfIndex,
    _In_ CONST XDP_HOOK_ID *HookIds,
    _In_ UINT32 HookCount,
    _In_opt_ XDP_INTERFACE_MODE *RequiredMode
    );

VOID
XdpIfDereferenceBinding(
    _In_ XDP_BINDING_HANDLE BindingHandle
    );

VOID
XdpIfQueueWorkItem(
    _In_ XDP_BINDING_WORKITEM *WorkItem
    );

CONST XDP_CAPABILITIES_INTERNAL *
XdpIfGetCapabilities(
    _In_ XDP_BINDING_HANDLE BindingHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
BOOLEAN
XdpIfSupportsHookId(
    _In_ CONST XDP_CAPABILITIES_INTERNAL *Capabilities,
    _In_ CONST XDP_HOOK_ID *Target
    );

//
// The following routines must be invoked from the serialized work queue.
//

//
// Interface removal event callback.
//
typedef
VOID
XDP_BINDING_DETACH_CALLBACK(
    _In_ XDP_BINDING_CLIENT_ENTRY *Client
    );

typedef enum _XDP_BINDING_CLIENT_ID {
    XDP_BINDING_CLIENT_ID_INVALID,
    XDP_BINDING_CLIENT_ID_RX_QUEUE,
    XDP_BINDING_CLIENT_ID_TX_QUEUE,
} XDP_BINDING_CLIENT_ID;

typedef struct _XDP_BINDING_CLIENT {
    XDP_BINDING_CLIENT_ID ClientId;
    UINT32 KeySize;
    XDP_BINDING_DETACH_CALLBACK *BindingDetached;
} XDP_BINDING_CLIENT;

typedef struct _XDP_BINDING_CLIENT_ENTRY {
    LIST_ENTRY Link;
    CONST XDP_BINDING_CLIENT *Client;
    CONST VOID *Key;
} XDP_BINDING_CLIENT_ENTRY;

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfInitializeClientEntry(
    _Out_ XDP_BINDING_CLIENT_ENTRY *ClientEntry
    );

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
XdpIfRegisterClient(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ CONST XDP_BINDING_CLIENT *Client,
    _In_ CONST VOID *Key,
    _In_ XDP_BINDING_CLIENT_ENTRY *ClientEntry
    );

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfDeregisterClient(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ XDP_BINDING_CLIENT_ENTRY *ClientEntry
    );

_IRQL_requires_(PASSIVE_LEVEL)
XDP_BINDING_CLIENT_ENTRY *
XdpIfFindClientEntry(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ CONST XDP_BINDING_CLIENT *Client,
    _In_ CONST VOID *Key
    );

_IRQL_requires_(PASSIVE_LEVEL)
NET_IFINDEX
XdpIfGetIfIndex(
    _In_ XDP_BINDING_HANDLE BindingHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
XdpIfCreateRxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _Inout_ XDP_RX_QUEUE_CONFIG_CREATE Config,
    _Out_ XDP_INTERFACE_HANDLE *InterfaceRxQueue,
    _Out_ CONST XDP_INTERFACE_RX_QUEUE_DISPATCH **InterfaceRxQueueDispatch
    );

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfActivateRxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ XDP_INTERFACE_HANDLE InterfaceRxQueue,
    _In_ XDP_RX_QUEUE_HANDLE XdpRxQueue,
    _In_ XDP_RX_QUEUE_CONFIG_ACTIVATE Config
    );

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfDeleteRxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ XDP_INTERFACE_HANDLE InterfaceRxQueue
    );

_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS
XdpIfCreateTxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _Inout_ XDP_TX_QUEUE_CONFIG_CREATE Config,
    _Out_ XDP_INTERFACE_HANDLE *InterfaceTxQueue,
    _Out_ CONST XDP_INTERFACE_TX_QUEUE_DISPATCH **InterfaceTxQueueDispatch
    );

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfActivateTxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ XDP_INTERFACE_HANDLE InterfaceTxQueue,
    _In_ XDP_TX_QUEUE_HANDLE XdpTxQueue,
    _In_ XDP_TX_QUEUE_CONFIG_ACTIVATE Config
    );

_IRQL_requires_(PASSIVE_LEVEL)
VOID
XdpIfDeleteTxQueue(
    _In_ XDP_BINDING_HANDLE BindingHandle,
    _In_ XDP_INTERFACE_HANDLE InterfaceTxQueue
    );

//
// End of serialized routines.
//

_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
NTSTATUS
XdpIfStart(
    VOID
    );

_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
VOID
XdpIfStop(
    VOID
    );