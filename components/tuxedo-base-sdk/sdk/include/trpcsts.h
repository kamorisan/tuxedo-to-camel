/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1993 Unix System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF Unix System Laboratories, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TRPCSTS_H
#define TRPCSTS_H
/* #ident	"@(#) trpc/rpc/trpcsts.h	$Revision: 1.3 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_trpcsts[] = "@(#) trpc/rpc/trpcsts.h	$Revision: 1.3 $";
#endif

#include <rpc/texc.h>

/* Failures which may be returned in a [comm_status] Parameter */
#define rpc_s_access_control_info_inv	0x16C9A04A
#define rpc_s_assoc_req_rejected	0x16C9A055
#define rpc_s_assoc_shutdown		0x16C9A056
#define rpc_s_binding_vector_full	0x16C9A0B9
#define rpc_s_cancel_timeout		0x16C9A030
#define rpc_s_cannot_accept		0x16C9A037
#define rpc_s_cant_bind_socket		0x16C9A003
#define rpc_s_cant_create_socket	0x16C9A002
#define rpc_s_cant_listen_socket	0x16C9A059
#define rpc_s_coding_error		0x16C9A01A
#define rpc_s_comm_failure		0x16C9A016
#define rpc_s_connect_closed_by_rem	0x16C9A04C
#define rpc_s_connect_no_resources	0x16C9A044
#define rpc_s_connect_rejected		0x16C9A042
#define rpc_s_connect_timed_out		0x16C9A041
#define rpc_s_connection_aborted	0x16C9A035
#define rpc_s_connection_closed		0x16C9A036
#define rpc_s_cycle_detected		0x16C9A0BA
#define rpc_s_database_busy		0x16C9A0AD
#define rpc_s_entry_not_found		0x16C9A0A0
#define rpc_s_group_not_found		0x16C9A095
#define rpc_s_host_unreachable		0x16C9A049
#define rpc_s_incomplete_name		0x16C9A094
#define rpc_s_invalid_binding		0x16C9A01D
#define rpc_s_invalid_call_opt		0x16C9A02E
#define rpc_s_invalid_endpoint_format	0x16C9A04E
#define rpc_s_invalid_import_context	0x16C9A0AE
#define rpc_s_invalid_inquiry_context	0x16C9A0A1
#define rpc_s_invalid_inquiry_type	0x16C9A0A9
#define rpc_s_invalid_lookup_context	0x16C9A0B8
#define rpc_s_invalid_name_syntax	0x16C9A096
#define rpc_s_invalid_object		0x16C9A03A
#define rpc_s_invalid_rpc_protseq	0x16C9A020
#define rpc_s_invalid_string_binding	0x16C9A040
#define rpc_s_invalid_vers_option	0x16C9A0BD
#define rpc_s_loc_connect_aborted	0x16C9A04B
#define rpc_s_manager_not_entered	0x16C9A06E
#define rpc_s_network_unreachable	0x16C9A043
#define rpc_s_no_fault			0x16C9A02F
#define rpc_s_no_memory			0x16C9A012
#define rpc_s_no_more_bindings		0x16C9A0B5
#define rpc_s_no_ns_priviledge		0x16C9A0A8
#define rpc_s_no_rem_endpoint		0x16C9A047
#define rpc_s_not_found			0x16C9A09C
#define rpc_s_not_rpc_entry		0x16C9A0B7
#define rpc_s_op_rng_error		0x16C9A001
#define rpc_s_profile_not_found		0x16C9A09B
#define rpc_s_protocol_error		0x16C9A03E
#define rpc_s_rem_host_crashed		0x16C9A04D
#define rpc_s_rem_host_down		0x16C9A048
#define rpc_s_rem_network_shutdown	0x16C9A045
#define rpc_s_too_many_rem_connects	0x16C9A046
#define rpc_s_tsyntaxes_unsupported	0x16C9A057
#define rpc_s_unknown_error		0x16C9A0DA
#define rpc_s_unknown_if		0x16C9A02C
#define rpc_s_unknown_ns_error		0x16C9A092
#define rpc_s_unknown_reject		0x16C9A060
#define rpc_s_unsupported_name_syntax	0x16C9A0A6
#define rpc_s_unsupported_protect_level	0x16C9A0E0
#define rpc_s_unsupported_type		0x16C9A02D
#define rpc_s_who_are_you_failed	0x16C9A05F
#define rpc_s_wrong_boot_time		0x16C9A006
#define uuid_s_invalid_string_uuid	0x16C9A08F


/* Exception codes associated with [comm_status] values */
#if (defined(_TM_OS2) || defined(_TM_WIN) || defined(WIN32)) && !defined(_TMDLL) && !defined(_TMEXCPTINIT)
#define rpc_x_access_control_info_inv	(*_tmexception(rpc_s_access_control_info_inv);)
#define rpc_x_assoc_req_rejected	(*_tmexception(rpc_s_assoc_req_rejected))
#define rpc_x_assoc_shutdown		(*_tmexception(rpc_s_assoc_shutdown))
#define rpc_x_binding_vector_full	(*_tmexception(rpc_s_binding_vector_full))
#define rpc_x_cancel_timeout		(*_tmexception(rpc_s_cancel_timeout))
#define rpc_x_cannot_accept		(*_tmexception(rpc_s_cannot_accept))
#define rpc_x_cant_bind_socket		(*_tmexception(rpc_s_cant_bind_socket))
#define rpc_x_cant_create_socket	(*_tmexception(rpc_s_cant_create_socket))
#define rpc_x_cant_listen_socket	(*_tmexception(rpc_s_cant_listen_socket))
#define rpc_x_coding_error		(*_tmexception(rpc_s_coding_error))
#define rpc_x_comm_failure		(*_tmexception(rpc_s_comm_failure))
#define rpc_x_connect_closed_by_rem	(*_tmexception(rpc_s_connect_closed_by_rem))
#define rpc_x_connect_no_resources	(*_tmexception(rpc_s_connect_no_resources))
#define rpc_x_connect_rejected		(*_tmexception(rpc_s_connect_rejected))
#define rpc_x_connect_timed_out		(*_tmexception(rpc_s_connect_timed_out))
#define rpc_x_connection_aborted	(*_tmexception(rpc_s_connection_aborted))
#define rpc_x_connection_closed		(*_tmexception(rpc_s_connection_closed))
#define rpc_x_cycle_detected		(*_tmexception(rpc_s_cycle_detected))
#define rpc_x_database_busy		(*_tmexception(rpc_s_database_busy))
#define rpc_x_entry_not_found		(*_tmexception(rpc_s_entry_not_found))
#define rpc_x_group_not_found		(*_tmexception(rpc_s_group_not_found))
#define rpc_x_host_unreachable		(*_tmexception(rpc_s_host_unreachable))
#define rpc_x_incomplete_name		(*_tmexception(rpc_s_incomplete_name))
#define rpc_x_invalid_binding		(*_tmexception(rpc_s_invalid_binding))
#define rpc_x_invalid_call_opt		(*_tmexception(rpc_s_invalid_call_opt))
#define rpc_x_invalid_endpoint_format	(*_tmexception(rpc_s_invalid_endpoint_format))
#define rpc_x_invalid_import_context	(*_tmexception(rpc_s_invalid_import_context))
#define rpc_x_invalid_inquiry_context	(*_tmexception(rpc_s_invalid_inquiry_context))
#define rpc_x_invalid_inquiry_type	(*_tmexception(rpc_s_invalid_inquiry_type))
#define rpc_x_invalid_lookup_context	(*_tmexception(rpc_s_invalid_lookup_context))
#define rpc_x_invalid_name_syntax	(*_tmexception(rpc_s_invalid_name_syntax))
#define rpc_x_invalid_object		(*_tmexception(rpc_s_invalid_object))
#define rpc_x_invalid_rpc_protseq	(*_tmexception(rpc_s_invalid_rpc_protseq))
#define rpc_x_invalid_string_binding	(*_tmexception(rpc_s_invalid_string_binding))
#define rpc_x_invalid_vers_option	(*_tmexception(rpc_s_invalid_vers_option))
#define rpc_x_loc_connect_aborted	(*_tmexception(rpc_s_loc_connect_aborted))
#define rpc_x_manager_not_entered	(*_tmexception(rpc_s_manager_not_entered))
#define rpc_x_network_unreachable	(*_tmexception(rpc_s_network_unreachable))
#define rpc_x_no_fault			(*_tmexception(rpc_s_no_fault))
#define rpc_x_no_memory			(*_tmexception(rpc_s_no_memory))
#define rpc_x_no_more_bindings		(*_tmexception(rpc_s_no_more_bindings))
#define rpc_x_no_ns_priviledge		(*_tmexception(rpc_s_no_ns_priviledge))
#define rpc_x_no_rem_endpoint		(*_tmexception(rpc_s_no_rem_endpoint))
#define rpc_x_not_found			(*_tmexception(rpc_s_not_found))
#define rpc_x_not_rpc_entry		(*_tmexception(rpc_s_not_rpc_entry))
#define rpc_x_op_rng_error		(*_tmexception(rpc_s_op_rng_error))
#define rpc_x_profile_not_found		(*_tmexception(rpc_s_profile_not_found))
#define rpc_x_protocol_error		(*_tmexception(rpc_s_protocol_error))
#define rpc_x_rem_host_crashed		(*_tmexception(rpc_s_rem_host_crashed))
#define rpc_x_rem_host_down		(*_tmexception(rpc_s_rem_host_down))
#define rpc_x_rem_network_shutdown	(*_tmexception(rpc_s_rem_network_shutdown))
#define rpc_x_too_many_rem_connects	(*_tmexception(rpc_s_too_many_rem_connects))
#define rpc_x_tsyntaxes_unsupported	(*_tmexception(rpc_s_tsyntaxes_unsupported))
#define rpc_x_unknown_error		(*_tmexception(rpc_s_unknown_error))
#define rpc_x_unknown_if		(*_tmexception(rpc_s_unknown_if))
#define rpc_x_unknown_ns_error		(*_tmexception(rpc_s_unknown_ns_error))
#define rpc_x_unknown_reject		(*_tmexception(rpc_s_unknown_reject))
#define rpc_x_unsupported_name_syntax	(*_tmexception(rpc_s_unsupported_name_syntax))
#define rpc_x_unsupported_protect_level	(*_tmexception(rpc_s_unsupported_protect_level))
#define rpc_x_unsupported_type		(*_tmexception(rpc_s_unsupported_type))
#define rpc_x_who_are_you_failed	(*_tmexception(rpc_s_who_are_you_failed))
#define rpc_x_wrong_boot_time		(*_tmexception(rpc_s_wrong_boot_time))
#define uuid_x_invalid_string_uuid	(*_tmexception(uuid_s_invalid_string_uuid))
#else
_TMEXCEPTINIT(rpc_x_access_control_info_inv,rpc_s_access_control_info_inv);
_TMEXCEPTINIT(rpc_x_assoc_req_rejected,rpc_s_assoc_req_rejected);
_TMEXCEPTINIT(rpc_x_assoc_shutdown,rpc_s_assoc_shutdown);
_TMEXCEPTINIT(rpc_x_binding_vector_full,rpc_s_binding_vector_full);
_TMEXCEPTINIT(rpc_x_cancel_timeout,rpc_s_cancel_timeout);
_TMEXCEPTINIT(rpc_x_cannot_accept,rpc_s_cannot_accept);
_TMEXCEPTINIT(rpc_x_cant_bind_socket,rpc_s_cant_bind_socket);
_TMEXCEPTINIT(rpc_x_cant_create_socket,rpc_s_cant_create_socket);
_TMEXCEPTINIT(rpc_x_cant_listen_socket,rpc_s_cant_listen_socket);
_TMEXCEPTINIT(rpc_x_coding_error,rpc_s_coding_error);
_TMEXCEPTINIT(rpc_x_comm_failure,rpc_s_comm_failure);
_TMEXCEPTINIT(rpc_x_connect_closed_by_rem,rpc_s_connect_closed_by_rem);
_TMEXCEPTINIT(rpc_x_connect_no_resources,rpc_s_connect_no_resources);
_TMEXCEPTINIT(rpc_x_connect_rejected,rpc_s_connect_rejected);
_TMEXCEPTINIT(rpc_x_connect_timed_out,rpc_s_connect_timed_out);
_TMEXCEPTINIT(rpc_x_connection_aborted,rpc_s_connection_aborted);
_TMEXCEPTINIT(rpc_x_connection_closed,rpc_s_connection_closed);
_TMEXCEPTINIT(rpc_x_cycle_detected,rpc_s_cycle_detected);
_TMEXCEPTINIT(rpc_x_database_busy,rpc_s_database_busy);
_TMEXCEPTINIT(rpc_x_entry_not_found,rpc_s_entry_not_found);
_TMEXCEPTINIT(rpc_x_group_not_found,rpc_s_group_not_found);
_TMEXCEPTINIT(rpc_x_host_unreachable,rpc_s_host_unreachable);
_TMEXCEPTINIT(rpc_x_incomplete_name,rpc_s_incomplete_name);
_TMEXCEPTINIT(rpc_x_invalid_binding,rpc_s_invalid_binding);
_TMEXCEPTINIT(rpc_x_invalid_call_opt,rpc_s_invalid_call_opt);
_TMEXCEPTINIT(rpc_x_invalid_endpoint_format,rpc_s_invalid_endpoint_format);
_TMEXCEPTINIT(rpc_x_invalid_import_context,rpc_s_invalid_import_context);
_TMEXCEPTINIT(rpc_x_invalid_inquiry_context,rpc_s_invalid_inquiry_context);
_TMEXCEPTINIT(rpc_x_invalid_inquiry_type,rpc_s_invalid_inquiry_type);
_TMEXCEPTINIT(rpc_x_invalid_lookup_context,rpc_s_invalid_lookup_context);
_TMEXCEPTINIT(rpc_x_invalid_name_syntax,rpc_s_invalid_name_syntax);
_TMEXCEPTINIT(rpc_x_invalid_object,rpc_s_invalid_object);
_TMEXCEPTINIT(rpc_x_invalid_rpc_protseq,rpc_s_invalid_rpc_protseq);
_TMEXCEPTINIT(rpc_x_invalid_string_binding,rpc_s_invalid_string_binding);
_TMEXCEPTINIT(rpc_x_invalid_vers_option,rpc_s_invalid_vers_option);
_TMEXCEPTINIT(rpc_x_loc_connect_aborted,rpc_s_loc_connect_aborted);
_TMEXCEPTINIT(rpc_x_manager_not_entered,rpc_s_manager_not_entered);
_TMEXCEPTINIT(rpc_x_network_unreachable,rpc_s_network_unreachable);
_TMEXCEPTINIT(rpc_x_no_fault,rpc_s_no_fault);
_TMEXCEPTINIT(rpc_x_no_memory,rpc_s_no_memory);
_TMEXCEPTINIT(rpc_x_no_more_bindings,rpc_s_no_more_bindings);
_TMEXCEPTINIT(rpc_x_no_ns_priviledge,rpc_s_no_ns_priviledge);
_TMEXCEPTINIT(rpc_x_no_rem_endpoint,rpc_s_no_rem_endpoint);
_TMEXCEPTINIT(rpc_x_not_found,rpc_s_not_found);
_TMEXCEPTINIT(rpc_x_not_rpc_entry,rpc_s_not_rpc_entry);
_TMEXCEPTINIT(rpc_x_op_rng_error,rpc_s_op_rng_error);
_TMEXCEPTINIT(rpc_x_profile_not_found,rpc_s_profile_not_found);
_TMEXCEPTINIT(rpc_x_protocol_error,rpc_s_protocol_error);
_TMEXCEPTINIT(rpc_x_rem_host_crashed,rpc_s_rem_host_crashed);
_TMEXCEPTINIT(rpc_x_rem_host_down,rpc_s_rem_host_down);
_TMEXCEPTINIT(rpc_x_rem_network_shutdown,rpc_s_rem_network_shutdown);
_TMEXCEPTINIT(rpc_x_too_many_rem_connects,rpc_s_too_many_rem_connects);
_TMEXCEPTINIT(rpc_x_tsyntaxes_unsupported,rpc_s_tsyntaxes_unsupported);
_TMEXCEPTINIT(rpc_x_unknown_error,rpc_s_unknown_error);
_TMEXCEPTINIT(rpc_x_unknown_if,rpc_s_unknown_if);
_TMEXCEPTINIT(rpc_x_unknown_ns_error,rpc_s_unknown_ns_error);
_TMEXCEPTINIT(rpc_x_unknown_reject,rpc_s_unknown_reject);
_TMEXCEPTINIT(rpc_x_unsupported_name_syntax,rpc_s_unsupported_name_syntax);
_TMEXCEPTINIT(rpc_x_unsupported_protect_level,rpc_s_unsupported_protect_level);
_TMEXCEPTINIT(rpc_x_unsupported_type,rpc_s_unsupported_type);
_TMEXCEPTINIT(rpc_x_who_are_you_failed,rpc_s_who_are_you_failed);
_TMEXCEPTINIT(rpc_x_wrong_boot_time,rpc_s_wrong_boot_time);
_TMEXCEPTINIT(uuid_x_invalid_string_uuid,uuid_s_invalid_string_uuid);
#endif

/* Failues which may be returned in a [fault_status] Parameter */
#define rpc_s_call_cancelled		0x16C9A031
#define rpc_s_fault_addr_error		0x16C9A074
#define rpc_s_fault_context_mismatch	0x16C9A075
#define rpc_s_fault_fp_div_by_zero	0x16C9A076
#define rpc_s_fault_fp_error		0x16C9A077
#define rpc_s_fault_fp_overflow		0x16C9A078
#define rpc_s_fault_fp_underflow	0x16C9A079
#define rpc_s_fault_ill_inst		0x16C9A07A
#define rpc_s_fault_int_div_by_zero	0x16C9A07B
#define rpc_s_fault_int_overflow	0x16C9A07C
#define rpc_s_fault_invalid_bound	0x16C9A07D
#define rpc_s_fault_invalid_tag		0x16C9A07E
#define rpc_s_fault_pipe_closed		0x16C9A07F
#define rpc_s_fault_pipe_comm_error	0x16C9A080
#define rpc_s_fault_pipe_discipline	0x16C9A081
#define rpc_s_fault_pipe_empty		0x16C9A082
#define rpc_s_fault_pipe_memory		0x16C9A083
#define rpc_s_fault_pipe_order		0x16C9A084
#define rpc_s_fault_remote_comm_failure	0x16C9A085
#define rpc_s_fault_remote_no_memory	0x16C9A086
#define rpc_s_fault_unspec		0x16C9A087

/* TxRPC */
#define rpc_s_tx_not_in_transaction	0x16C9A115
#define rpc_s_tx_open_failed		0x16C9A116
#define txrpc_s_no_tx_open_done		rpc_s_tx_open_failed
#define txrpc_s_not_in_transaction	rpc_s_tx_not_in_transaction

/* Exceptions associated with [fault_status] values */
#if (defined(_TM_OS2) || defined(_TM_WIN) || defined(WIN32)) && !defined(_TMDLL) && !defined(_TMEXCPTINIT)
#define rpc_x_call_cancelled		(*_tmexception(rpc_s_call_cancelled))
#define rpc_x_fault_addr_error		(*_tmexception(rpc_s_fault_addr_error))
#define rpc_x_fault_context_mismatch	(*_tmexception(rpc_s_fault_context_mismatch))
#define rpc_x_fault_fp_div_by_zero	(*_tmexception(rpc_s_fault_fp_div_by_zero))
#define rpc_x_fault_fp_error		(*_tmexception(rpc_s_fault_fp_error))
#define rpc_x_fault_fp_overflow		(*_tmexception(rpc_s_fault_fp_overflow))
#define rpc_x_fault_fp_underflow	(*_tmexception(rpc_s_fault_fp_underflow))
#define rpc_x_fault_ill_inst		(*_tmexception(rpc_s_fault_ill_inst))
#define rpc_x_fault_int_div_by_zero	(*_tmexception(rpc_s_fault_int_div_by_zero))
#define rpc_x_fault_int_overflow	(*_tmexception(rpc_s_fault_int_overflow))
#define rpc_x_fault_invalid_bound	(*_tmexception(rpc_s_fault_invalid_bound))
#define rpc_x_fault_invalid_tag		(*_tmexception(rpc_s_fault_invalid_tag))
#define rpc_x_fault_pipe_closed		(*_tmexception(rpc_s_fault_pipe_closed))
#define rpc_x_fault_pipe_comm_error	(*_tmexception(rpc_s_fault_pipe_comm_error))
#define rpc_x_fault_pipe_discipline	(*_tmexception(rpc_s_fault_pipe_discipline))
#define rpc_x_fault_pipe_empty		(*_tmexception(rpc_s_fault_pipe_empty))
#define rpc_x_fault_pipe_memory		(*_tmexception(rpc_s_fault_pipe_memory))
#define rpc_x_fault_pipe_order		(*_tmexception(rpc_s_fault_pipe_order))
#define rpc_x_fault_remote_comm_failure	(*_tmexception(rpc_s_fault_remote_comm_failure))
#define rpc_x_fault_remote_no_memory	(*_tmexception(rpc_s_fault_remote_no_memory))
#define rpc_x_fault_unspec		(*_tmexception(rpc_s_fault_unspec))
#define txrpc_x_no_tx_open_done		(*_tmexception(txrpc_s_no_tx_open_done))
#define txrpc_x_not_in_transaction	(*_tmexception(txrpc_s_not_in_transaction))
#else
_TMEXCEPTINIT(rpc_x_call_cancelled,rpc_s_call_cancelled);
_TMEXCEPTINIT(rpc_x_fault_addr_error,rpc_s_fault_addr_error);
_TMEXCEPTINIT(rpc_x_fault_context_mismatch,rpc_s_fault_context_mismatch);
_TMEXCEPTINIT(rpc_x_fault_fp_div_by_zero,rpc_s_fault_fp_div_by_zero);
_TMEXCEPTINIT(rpc_x_fault_fp_error,rpc_s_fault_fp_error);
_TMEXCEPTINIT(rpc_x_fault_fp_overflow,rpc_s_fault_fp_overflow);
_TMEXCEPTINIT(rpc_x_fault_fp_underflow,rpc_s_fault_fp_underflow);
_TMEXCEPTINIT(rpc_x_fault_ill_inst,rpc_s_fault_ill_inst);
_TMEXCEPTINIT(rpc_x_fault_int_div_by_zero,rpc_s_fault_int_div_by_zero);
_TMEXCEPTINIT(rpc_x_fault_int_overflow,rpc_s_fault_int_overflow);
_TMEXCEPTINIT(rpc_x_fault_invalid_bound,rpc_s_fault_invalid_bound);
_TMEXCEPTINIT(rpc_x_fault_invalid_tag,rpc_s_fault_invalid_tag);
_TMEXCEPTINIT(rpc_x_fault_pipe_closed,rpc_s_fault_pipe_closed);
_TMEXCEPTINIT(rpc_x_fault_pipe_comm_error,rpc_s_fault_pipe_comm_error);
_TMEXCEPTINIT(rpc_x_fault_pipe_discipline,rpc_s_fault_pipe_discipline);
_TMEXCEPTINIT(rpc_x_fault_pipe_empty,rpc_s_fault_pipe_empty);
_TMEXCEPTINIT(rpc_x_fault_pipe_memory,rpc_s_fault_pipe_memory);
_TMEXCEPTINIT(rpc_x_fault_pipe_order,rpc_s_fault_pipe_order);
_TMEXCEPTINIT(rpc_x_fault_remote_comm_failure,rpc_s_fault_remote_comm_failure);
_TMEXCEPTINIT(rpc_x_fault_remote_no_memory,rpc_s_fault_remote_no_memory);
_TMEXCEPTINIT(rpc_x_fault_unspec,rpc_s_fault_unspec);
_TMEXCEPTINIT(txrpc_x_no_tx_open_done,txrpc_s_no_tx_open_done);
_TMEXCEPTINIT(txrpc_x_not_in_transaction,txrpc_s_not_in_transaction);
#endif

#endif
