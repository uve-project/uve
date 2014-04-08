// This module implements the QsciLexerSystemVerilog class.
//
// Copyright (c) 2011 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexersystemverilog.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerSystemVerilog::QsciLexerSystemVerilog(QObject *parent)
    : QsciLexer(parent),
      fold_atelse(false), fold_comments(false), fold_compact(true),
      fold_preproc(false), fold_atmodule(false)
{
}


// The dtor.
QsciLexerSystemVerilog::~QsciLexerSystemVerilog()
{
}


// Returns the language name.
const char *QsciLexerSystemVerilog::language() const
{
    return "systemverilog";
}


// Returns the lexer name.
const char *QsciLexerSystemVerilog::lexer() const
{
    return "systemverilog";
}


// Return the style used for braces.
int QsciLexerSystemVerilog::braceStyle() const
{
    return Operator;
}


// Returns the set of keywords.
const char *QsciLexerSystemVerilog::keywords(int set) const
{
    if (set == 1)
        return
			"alias always always_comb always_ff always_latch and assert assign assume "
			"automatic before begin bind bins binsof bit break buf bufif0 bufif1 byte "
			"case casex casez cell chandle class clocking cmos config consts constraint "
			"context continue cover covergroup coverpoint corss deassign default defparam "
			"design disable dist do edge else end endcase endclass endclocking endconfig "
			"endfunction endgenerate endgroup endinterface endmodule endpackage endprimitive "
			"endprogram endproperty endspecify endsequence endtable endtask enum event "
			"expect export extends extern final first_match for force freach forever fork "
			"forkjoin function generate genvar highz0 highz1 if iff ifnone ignore_bins "
			"illegal_bins import incdir include initial inout input inside instance "
			"int integer interface intersect join join_any join_none large liblist library "
			"local localparam logic longint macromodule matches medium modport module nand "
			"negedge new nmos nor noshowcancelled not notif0 notif1 null or output package "
			"packed parameter pmos posedge primitive priority program property protected "
			"pull0 pull1 pulldown pullup pulsestyle_onevent pulsestyle_ondetect pure "
			"rand randc randcase randsequence rcmos real realtime ref reg release repeat "
			"return rnmos rpmos rtran rtranif0 rtranif1 scalared sequence shortint shortreal "
			"showcancelled signed small solve specify specparam static string strong0 "
			"strong1 struct super supply0 supply1 table tagged task this throughout time "
			"timeprecision timeunit tran tranif0 tranif1 tri tri tri1 triand trior trireg "
			"type typedef union unique unsigned use var vectored virtual void wait "
			"wait_order wand weak0 weak1 while wildcard wire with within wor xnor xor";
		
	if (set == 3)
        return
			"$assertkill $assertoff $asserton $async$and$array $async$nand$array $async$or$array $async$nor$array "
			"$async$and$plane $async$nand$plane $async$or$plane $async$nor$plane $sync$and$array $sync$nand$array "
			"$sync$or$array $sync$nor$array $sync$and$plane $sync$nand$plane $sync$or$plane $sync$nor$plane $bits "
			"$bitstoreal $bitstoshortreal $case $comment $countdrivers $date $dimensions $display $write $dist_chi_square "
			"$dist_erlang $dist_exponential $dist_normal $dist_poisson $dist_t $dist_uniform $dumpall $dumpfile "
			"$dumpfile $dumplimit $dumpoff $dumpon $dumpports $dumpportsall $dumpportsflush $dumpportlimit $dumpportsoff "
			"$dumpportson $dumpvar $enddefinitions $error $exit $fatal $fdisplay $fell $feof $ferror $fflush $fgetc "
			"$finish $fopen $fclose $fread $fscanf $fseek $ftell $fullskew $fwrite $get_coverage $getpattern $high "
			"$hold $increment $incsave $info $input $isunbounded $isunknow $itor $key $left $list $load_coverage_db "
			"$log $low $monitor $fmonitor $monitoron $monitoroff $nochange $nokey $nolog $onehot $onehot0 $past "
			"$period $printtimescale $q_add $q_exam $q_full $q_initialize $q_remove $random $readmemb/h $writememb/h "
			"$realtime $realtoits $recovery $recrem $removal $reset $reset_count $reset_value $restart $rewind $right "
			"$rose $rtoi $sampled $save $scale $scope $set_coverage_db_name $suted $sformat $shortrealtobits "
			"$showscopes $showvars $signed $size $skew $sreadmemh $sscanf $stable $stime $stop $strobe $swrite "
			"$test$plusargs $time $timeformat $timescale $timeskew $typename $typeof $uandom $ungetc $undigned "
			"$upscope $urandom_range $value$plusargs $var $vcdclose $version $warning $width $write";
			
	if (set == 4)
        return		
			"uvm_agent uvm_algorithmic_comparator "
			"uvm_analysis_export uvm_analysis_imp uvm_analysis_port uvm_barrier uvm_bottomup_phase uvm_build_phase "
			"uvm_built_in_clone uvm_built_in_comp uvm_built_in_converter uvm_built_in_pair "
			"uvm_callback uvm_callback_iter uvm_callbacks uvm_callbacks_objection uvm_check_phase "
			"uvm_class_clone uvm_class_comp uvm_class_converter uvm_class_pair uvm_cmdline_processor "
			"uvm_comparer uvm_component uvm_component_registry uvm_config_db uvm_config_db_options "
			"uvm_configure_phase uvm_connect_phase uvm_domain uvm_driver uvm_end_of_elaboration_phase "
			"uvm_env uvm_event uvm_event_callback uvm_extract_phase uvm_factory uvm_final_phase uvm_hdl_path_concat "
			"uvm_heartbeat uvm_in_order_built_in_comparator uvm_in_order_class_comparator "
			"uvm_in_order_comparator uvm_line_printer uvm_main_phase uvm_mem "
			"uvm_mem_access_seq uvm_mem_mam uvm_mem_mam_cfg uvm_mem_mam_policy uvm_mem_region uvm_mem_shared_access_seq "
			"uvm_mem_single_access_seq uvm_mem_single_walk_seq uvm_mem_walk_seq uvm_monitor uvm_object uvm_object_registry "
			"uvm_object_string_pool uvm_object_wrapper uvm_objection uvm_objection_callback uvm_packer uvm_phase "
			"uvm_pool uvm_port_base uvm_port_component uvm_port_component_base uvm_post_configure_phase "
			"uvm_post_main_phase uvm_post_reset_phase uvm_post_shutdown_phase uvm_pre_configure_phase uvm_pre_main_phase "
			"uvm_pre_reset_phase uvm_pre_shutdown_phase uvm_printer uvm_printer_knobs uvm_push_driver "
			"uvm_push_sequencer uvm_queue uvm_random_stimulus uvm_recorder uvm_reg uvm_reg_access_seq "
			"uvm_reg_adapter uvm_reg_backdoor uvm_reg_bit_bash_seq uvm_reg_block uvm_reg_bus_op uvm_reg_cbs uvm_reg_field "
			"uvm_reg_fifo uvm_reg_file uvm_reg_frontdoor uvm_reg_hw_reset_seq uvm_reg_indirect_data uvm_reg_item uvm_reg_map "
			"uvm_reg_mem_access_seq uvm_reg_mem_built_in_seq uvm_reg_mem_hdl_paths_seq uvm_reg_mem_shared_access_seq "
			"uvm_reg_predictor uvm_reg_read_only_cbs uvm_reg_sequence uvm_reg_shared_access_seq uvm_reg_single_access_seq "
			"uvm_reg_single_bit_bash_seq uvm_reg_tlm_adapter uvm_reg_write_only_cbs uvm_report_catcher uvm_report_handler "
			"uvm_report_object uvm_report_phase uvm_report_server uvm_reset_phase uvm_resource uvm_resource_base "
			"uvm_resource_db uvm_resource_db_options uvm_resource_options uvm_resource_pool uvm_resource_types "
			"uvm_root uvm_run_phase uvm_scoreboard uvm_seq_item_pull_export uvm_seq_item_pull_imp "
			"uvm_seq_item_pull_port uvm_sequence uvm_sequence_base uvm_sequence_item "
			"uvm_sequencer uvm_sequencer_base uvm_sequencer_param_base uvm_shutdown_phase "
			"uvm_sqr_if_base uvm_start_of_simulation_phase uvm_subscriber uvm_table_printer "
			"uvm_task_phase uvm_test uvm_tlm_analysis_fifo uvm_tlm_b_initiator_socket uvm_tlm_b_initiator_socket_base "
			"uvm_tlm_b_passthrough_initiator_socket uvm_tlm_b_passthrough_initiator_socket_base uvm_tlm_b_passthrough_target_socket "
			"uvm_tlm_b_passthrough_target_socket_base uvm_tlm_b_target_socket uvm_tlm_b_target_socket_base "
			"uvm_tlm_b_transport_export uvm_tlm_b_transport_imp uvm_tlm_b_transport_port uvm_tlm_extension "
			"uvm_tlm_extension_base uvm_tlm_fifo uvm_tlm_fifo_base uvm_tlm_generic_payload uvm_tlm_gp uvm_tlm_if "
			"uvm_tlm_if_base uvm_tlm_nb_initiator_socket uvm_tlm_nb_initiator_socket_base "
			"uvm_tlm_nb_passthrough_initiator_socket uvm_tlm_nb_passthrough_initiator_socket_base uvm_tlm_nb_passthrough_target_socket "
			"uvm_tlm_nb_passthrough_target_socket_base uvm_tlm_nb_target_socket uvm_tlm_nb_target_socket_base "
			"uvm_tlm_nb_transport_bw_export uvm_tlm_nb_transport_bw_imp uvm_tlm_nb_transport_bw_port "
			"uvm_tlm_nb_transport_fw_export uvm_tlm_nb_transport_fw_imp uvm_tlm_nb_transport_fw_port uvm_tlm_req_rsp_channel "
			"uvm_tlm_time uvm_tlm_transport_channel uvm_topdown_phase uvm_transaction "
			"uvm_tree_printer uvm_utils uvm_vreg uvm_vreg_cbs uvm_vreg_field uvm_vreg_field_cbs";
	
	if (set == 5)
        return	
			"accept_tr add add_by_name add_callback add_coverage add_hdl_path add_hdl_path_slice add_mem add_path add_reg add_slice add_submap "
			"add_uvm_phases adjust_name all_dropped allocate apply_config_settings b_transport backdoor_read backdoor_read_func backdoor_watch "
			"backdoor_write begin_child_tr begin_tr body build_coverage build_phase burst_read burst_write bus2reg "
			"callback_mode can_get can_peek can_put cancel capacity catch check_config_usage check_data_width "
			"check_phase clear clear_extension clear_extensions clear_hdl_path clear_response_queue clone compare "
			"compare_field compare_field_int compare_field_real compare_object compare_string compose_message "
			"configure configure_phase connect connect_phase convert2string copy create create_component "
			"create_component_by_name create_component_by_type create_item create_map create_object create_object_by_name "
			"create_object_by_type current_grabber debug_connected_to debug_create_by_name debug_create_by_type debug_provided_to "
			"decode decr define_access define_domain delete delete_by_name delete_callback die "
			"disable_recording display display_objections do_accept_tr do_begin_tr do_block do_bus_read "
			"do_bus_write do_compare do_copy do_end_tr do_kill do_pack do_post_read do_post_write do_pre_read "
			"do_pre_write do_predict do_print do_read do_record do_reg_item do_unpack do_write drop_objection "
			"dropped dump  dump_report_state dump_server_state emit enable_recording encode end_of_elaboration_phase "
			"end_tr exec_func exec_task execute execute_item exists extract_phase final_phase find "
			"find_all find_block find_blocks find_by_name find_override_by_name find_override_by_type find_unused_resources "
			"finish_item first flush for_each format_action format_header format_row generate_stimulus "
			"get get_abstime get_accept_time get_access get_action get_adapter get_addr_unit_bytes get_address get_addresses "
			"get_arbitration get_arg_matches get_arg_value get_arg_values get_args get_auto_predict get_backdoor get_base_addr "
			"get_begin_time get_block_by_name get_blocks get_by_name get_by_type get_byte_enable get_byte_enable_length "
			"get_cb get_check_on_read get_child get_children get_client get_command get_common_domain get_comp get_compare "
			"get_config get_config_int get_config_object get_config_string get_connected_to get_coverage get_current_item "
			"get_data get_data_length get_default_hdl_path get_default_path get_depth get_domain get_domain_name get_domains "
			"get_drain_time get_end_offset get_end_time get_event_pool get_extension get_field_by_name get_fields get_file_handle "
			"get_first get_first_child get_fname get_frontdoor get_full_hdl_path get_full_name get_global get_global_pool "
			"get_global_queue get_hdl_path g et_hdl_path_kinds get_highest_precedence get_id get_id_count get_if get_imp "
			"get_incr get_initiator get_inst get_inst_count get_inst_id get_is_active get_item get_jump_target get_last "
			"get_len get_line get_lsb_pos get_lsb_pos_in_register get_map_by_name get_maps get_max_quit_count get_max_size "
			"get_mem_by_name get_mem_by_offset get_memories get_memory get_message get_mirrored_value get_n_bits get_n_bytes "
			"get_n_maps get_n_memlocs get_name get_next get_next_child get_next_item get_num_children get_num_extensions "
			"get_num_last_reqs get_num_last_rsps get_num_reqs_sent get_num_rsps_received get_num_waiters get_object_type "
			"get_objection get_objection_count get_objection_total get_objectors get_offset get_offset_in_memory get_packed_size "
			"get_parent get_parent_map get_parent_sequence get_phase_type get_physical_addresses get_plusargs get_port "
			"get_prev get_priority get_provided_to get_quit_count get_radix_str get_realtime get_reg_by_name get_reg_by_offset "
			"get_regfile get_region get_registers get_report_action get_report_catcher get_report_file_handle get_report_handler "
			"get_report_server get_report_verbosity_level get_reset get_response get_response_queue_depth  "
			"get_response_queue_error_report_disabled get_response_status get_response_string get_rights get_root_blocks "
			"get_root_map get_root_sequence get_root_sequence_name get_run_count get_schedule get_schedule_name get_scope "
			"get_sequence_id get_sequence_path get_sequence_state get_sequencer get_server get_severity get_severity_count get_size "
			"get_start_offset get_state get_streaming_width get_submap_offset get_submaps get_threshold get_tool_name "
			"get_tool_version get_tr_handle get_transaction_id get_trigger_data get_trigger_time get_type get_type_handle  "
			"get_type_handle_name get_type_name get_use_response_handler get_use_sequence_info get_uvm_domain "
			"get_uvm_schedule get_uvmargs get_verbosity get_verbosity_level get_vfield_by_name get_virtual_fields "
			"get_virtual_registers get_vreg_by_name get_vreg_by_offset grab has_child has_coverage has_do_available "
			"has_hdl_path has_lock has_reset ID implement include_coverage incr incr_id_count incr_quit_count  "
			"incr_severity_count init_access_record insert is is_active is_after is_auditing is_auto_updated "
			"is_before is_blocked is_busy is_child is_dmi_allowed is_empty is_enabled is_export is_full "
			"is_grabbed is_hdl_path_root is_imp is_in_map is_indv_accessible is_item is_known_access "
			"is_locked is_null is_off is_on is_port is_quit_count_reached is_read is_read_only is_recording_enabled "
			"is_relevant is_response_error is_response_ok is_tracing is_unbounded is_volatile is_write issue "
			"item_done jump jump_all kill last last_req last_rsp lock lock_model lookup lookup_name lookup_regex "
			"min_size mirror  mirror_reg nb_transport nb_transport_bw nb_transport_fw n eeds_update "
			"lookup_regex_names lookup_scope lookup_type m_set_hier_mode main_phase match_scope max_size mid_do "
			"new next num pack pack_bytes pack_field pack_field_int pack_ints pack_object pack_real "
			"pack_string pack_time peek peek_mem peek_reg phase_ended phase_ready_to_end phase_started "
			"poke poke_mem poke_reg pop_back pop_front post_body post_configure_phase post_do "
			"post_main_phase post_predict post_read post_reset_phase post_shutdown_phase post_start "
			"post_trigger post_write pre_abort pre_body pre_configure_phase pre_do pre_main_phase "
			"pre_predict pre_read pre_reset_phase pre_shutdown_phase pre_start pre_trigger "
			"pre_write predict prev print print_accessors print_array_footer print_array_header "
			"print_array_range print_catcher print_config print_config_settings print_config_with_audit "
			"print_generic print_int print_msg print_object print_override_info print_resources print_string "
			"print_time print_topology process_report push_back push_front put "
			"raise_objection raised read read_by_name read_by_type read_func read_mem read_mem_by_name read_reg "
			"read_reg_by_name reconfigure record record_error_tr record_event_tr record_field record_field_real "
			"record_generic record_object record_read_access record_string record_time record_write_access reg2bus "
			"register release_all_regions release_region remove report report_error_hook report_fatal_hook report_header "
			"report_hook report_info_hook report_phase report_summarize report_warning_hook request_region reseed reserve_region "
			"reset reset_blk reset_phase reset_quit_count reset_report_handler reset_severity_counts resolve_bindings "
			"response_handler resume run_hooks run_phase run_test "
			"sample sample_values send_request set set_priority set_abstime set_access set_action set_address "
			"set_anonymous set_arbitration set_auto_predict set_auto_reset set_backdoor set_base_addr set_byte_enable "
			"set_byte_enable_length set_check_on_read set_command set_compare set_config_int set_config_object set_config_string "
			"set_coverage set_data set_data_length set_default set_default_hdl_path set_default_index set_default_map "
			"set_depth set_dmi_allowed set_domain set_drain_time set_extension set_frontdoor set_hdl_path_root "
			"set_heartbeat set_id set_id_count set_id_info set_initiator set_inst_override set_inst_override_by_name "
			"set_inst_override_by_type set_int_local set_item_context set_max_quit_count set_message set_mode "
			"set_name set_name_override set_num_last_reqs set_num_last_rsps set_object_local set_offset "
			"set_override set_parent_sequence set_phase_imp set_priority set_priority_name set_priority_type set_quit_count "
			"set_read set_read_only set_report_default_file set_report_default_file_hier set_report_handler set_report_id_action "
			"set_report_id_action_hier set_report_id_file set_report_id_file_hier set_report_id_verbosity set_report_id_verbosity_hier "
			"set_report_max_quit_count set_report_severity_action set_report_severity_action_hier set_report_severity_file "
			"set_report_severity_file_hier set_report_severity_id_action set_report_severity_id_action_hier set_report_severity_id_file "
			"set_report_severity_id_file_hier set_report_severity_id_override set_report_severity_id_verbosity "
			"set_report_severity_id_verbosity_hier set_report_severity_override set_report_verbosity_level set_report_verbosity_level_hier "
			"set_reset set_response_queue_depth set_response_queue_error_report_disabled set_response_status set_scope "
			"set_sequencer set_server set_severity set_severity_count set_streaming_width set_string_local set_submap_offset "
			"set_threshold set_time_resolution set_timeout set_transaction_id set_type_override set_type_override_by_name "
			"set_type_override_by_type set_use_sequence_info set_verbosity set_volatility set_write shutdown_phase size "
			"sort_by_precedence spell_check sprint start start_item start_of_simulation_phase start_phase_sequence stop "
			"stop_sequences stop_stimulus_generation summarize summarize_report_catcher suspend sync  unsync trace_mode "
			"transport traverse trigger try_get try_next_item try_peek try_put turn_off_auditing "
			"turn_off_tracing turn_on_auditing turn_on_tracing ungrab unlock unpack unpack_bytes unpack_field "
			"unpack_field_int unpack_ints unpack_object unpack_real unpack_string unpack_time unsync update "
			"update_reg use_response_handler used user_priority_arbitration uvm_bits_to_string uvm_hdl_check_path "
			"uvm_hdl_deposit uvm_hdl_force uvm_hdl_force_time uvm_hdl_read uvm_hdl_release uvm_hdl_release_and_read "
			"uvm_is_match uvm_report_enabled uvm_report_error uvm_report_fatal uvm_report_info uvm_report_warning "
			"uvm_split_string uvm_string_to_bits uvm_wait_for_nba_region wait_for wait_for_change wait_for_grant "
			"wait_for_item_done wait_for_relevant wait_for_sequence_state wait_for_sequences wait_for_state "
			"wait_modified wait_off wait_on wait_ptrigger wait_ptrigger_data wait_trigger wait_trigger_data "
			"write write_by_name write_by_type write_mem write_mem_by_name write_reg write_reg_by_name";

	if (set == 6)
        return	
			"`uvm_analysis_imp_decl `uvm_blocking_get_imp_decl `uvm_blocking_get_peek_imp_decl `uvm_blocking_master_imp_decl "
			"`uvm_blocking_peek_imp_decl `uvm_blocking_put_imp_decl `uvm_blocking_slave_imp_decl `uvm_blocking_transport_imp_decl "
			"`uvm_component_end `uvm_component_param_utils `uvm_component_param_utils_begin `uvm_component_registry "
			"`uvm_component_utils `uvm_component_utils_begin `uvm_create `uvm_create_on `uvm_declare_p_sequencer "
			"`UVM_DEFAULT_TIMEOUT `uvm_do `uvm_do_callbacks `uvm_do_callbacks_exit_on `uvm_do_obj_callbacks "
			"`uvm_do_obj_callbacks_exit_on `uvm_do_on `uvm_do_on_pri `uvm_do_on_pri_with `uvm_do_on_with `uvm_do_pri "
			"`uvm_do_pri_with `uvm_do_with `uvm_error `uvm_error_context `uvm_fatal `uvm_fatal_context "
			"`uvm_field_aa_int_byte `uvm_field_aa_int_byte_unsigned `uvm_field_aa_int_enumkey `uvm_field_aa_int_int "
			"`uvm_field_aa_int_int_unsigned `uvm_field_aa_int_integer `uvm_field_aa_int_integer_unsigned `uvm_field_aa_int_key "
			"`uvm_field_aa_int_longint `uvm_field_aa_int_longint_unsigned `uvm_field_aa_int_shortint `uvm_field_aa_int_shortint_unsigned "
			"`uvm_field_aa_int_string `uvm_field_aa_object_int `uvm_field_aa_object_string `uvm_field_aa_string_string "
			"`uvm_field_array_enum `uvm_field_array_int `uvm_field_array_object `uvm_field_array_string "
			"`uvm_field_enum `uvm_field_event `uvm_field_int `uvm_field_object `uvm_field_queue_enum "
			"`uvm_field_queue_int `uvm_field_queue_object `uvm_field_queue_string `uvm_field_real `uvm_field_sarray_enum "
			"`uvm_field_sarray_int `uvm_field_sarray_object `uvm_field_sarray_string `uvm_field_string `uvm_field_utils_begin "
			"`uvm_field_utils_end `uvm_get_imp_decl `uvm_get_peek_imp_decl `uvm_info `uvm_info_context `uvm_master_imp_decl "
			"`UVM_MAX_STREAMBITS `uvm_nonblocking_get_imp_decl `uvm_nonblocking_get_peek_imp_decl `uvm_nonblocking_master_imp_decl "
			"`uvm_nonblocking_peek_imp_decl `uvm_nonblocking_put_imp_decl `uvm_nonblocking_slave_imp_decl `uvm_nonblocking_transport_imp_decl "
			"`uvm_object_param_utils `uvm_object_param_utils_begin `uvm_object_registry `uvm_object_utils `uvm_object_utils_begin "
			"`uvm_object_utils_end `uvm_pack_array `uvm_pack_arrayN `uvm_pack_enum `uvm_pack_enumN `uvm_pack_int "
			"`uvm_pack_intN `uvm_pack_queue `uvm_pack_queueN `uvm_pack_real `uvm_pack_sarray `uvm_pack_sarrayN `uvm_pack_string "
			"`UVM_PACKER_MAX_BYTES `uvm_peek_imp_decl `uvm_put_imp_decl `uvm_rand_send `uvm_rand_send_pri `uvm_rand_send_pri_with "
			"`uvm_rand_send_with `uvm_record_attribute `uvm_record_field `UVM_REG_ADDR_WIDTH `UVM_REG_BYTENABLE_WIDTH `UVM_REG_CVR_WIDTH "
			"`UVM_REG_DATA_WIDTH `uvm_register_cb `uvm_send `uvm_send_pri `uvm_set_super_type `uvm_slave_imp_decl `UVM_TLM_B_MASK "
			"`UVM_TLM_B_TRANSPORT_IMP `UVM_TLM_FUNCTION_ERROR `UVM_TLM_NB_BW_MASK `UVM_TLM_NB_FW_MASK `UVM_TLM_NB_TRANSPORT_BW_IMP "
			"`UVM_TLM_NB_TRANSPORT_FW_IMP `UVM_TLM_TASK_ERROR `uvm_transport_imp_decl `uvm_unpack_array `uvm_unpack_arrayN "
			"`uvm_unpack_enum `uvm_unpack_enumN `uvm_unpack_int `uvm_unpack_intN `uvm_unpack_queue `uvm_unpack_queueN "
			"`uvm_unpack_real `uvm_unpack_sarray `uvm_unpack_sarrayN `uvm_unpack_string `uvm_warning `uvm_warning_context "
			//! all uvm port
			"after_export analysis_export analysis_export before_export blocking_put_port get_ap "
			"get_peek_export get_peek_request_export get_peek_response_export master_export pair_ap "
			"put_ap put_export put_request_export put_response_export req_export req_port request_ap "
			"response_ap rsp_export rsp_port seq_item_port slave_export transport_export "
			//! all uvm type	
			"alloc_mode_e locality_e uvm_mem_mam uvm_mem_mam uvm_access_e uvm_action uvm_active_passive_enum uvm_bitstream_t "
			"uvm_check_e uvm_coverage_model_e uvm_elem_kind_e uvm_endianness_e uvm_hdl_path_slice uvm_hier_e uvm_mem_cb"
			"uvm_mem_cb_iter uvm_objection_event uvm_path_e uvm_phase_state uvm_phase_transition uvm_phase_type uvm_port_type_e "
			"uvm_predict_e uvm_radix_enum uvm_recursion_policy_enum uvm_reg_addr_logic_t uvm_reg_addr_t uvm_reg_bd_cb "
			"uvm_reg_bd_cb_iter uvm_reg_byte_en_t uvm_reg_cb uvm_reg_cb_iter uvm_reg_cvr_t uvm_reg_data_logic_t uvm_reg_data_t "
			"uvm_reg_field_cb uvm_reg_field_cb_iter uvm_reg_mem_tests_e uvm_sequence_lib_mode uvm_sequence_state_enum uvm_sequencer_arb_mode "
			"uvm_severity uvm_status_e uvm_tlm_command_e uvm_tlm_phase_e uvm_tlm_response_status_e uvm_tlm_sync_e uvm_verbosity "
			"uvm_vreg_cb uvm_vreg_cb_iter uvm_vreg_field_cb uvm_vreg_field_cb_iter uvm_wait_op "
			//! all uvm variable
			"+UVM_CONFIG_DB_TRACE +UVM_DUMP_CMDLINE_ARGS +UVM_MAX_QUIT_COUNT +UVM_OBJECTION_TRACE +UVM_PHASE_TRACE "
			"+UVM_RESOURCE_DB_TRACE +uvm_set_action +uvm_set_config_int +uvm_set_config_string +uvm_set_inst_override "
			"+uvm_set_type_override +uvm_set_severity +uvm_set_verbosity +UVM_TESTNAME +UVM_TIMEOUT +UVM_VERBOSITY "
			"abstract abstractions adapter addr bd_kind begin_elements begin_event big_endian bin_radix body "
			"bus_in byte_en check_type data dec_radix default_alloc default_map default_path default_precedence default_radix "
			"depth do_not_randomize element element_kind enable_print_topology end_elements end_event "
			"end_offset events extension fifo finish_on_completion fname footer full_name header "
			"hex_radix id_count identifier in_use indent kind knobs len lineno local_map locality m_address m_byte_enable "
			"m_byte_enable_length m_command m_data m_dmi m_length m_response_status m_streaming_width mam map max_offset "
			"mcd mem mem_seq min_offset miscompares mode model n_bits n_bytes new oct_radix offset parent parent_sequence "
			"path physical policy precedence prefix print_config_matches print_enabled prior provides_responses recorder recursion_policy "
			"reference reg_ap reg_seq reg_seqr result rg rw_info separator seq_item_export sequencer sev show_max show_radix "
			"show_root size slices start_offset starting_phase status supports_babyte_enable T1_first T2_second tests "
			"top_levels tr_handle type_name unsigned_radix use_metadata use_uvm_seeding uvm_default_comparer uvm_default_line_printer "
			"uvm_default_packer uvm_default_printer uvm_default_recorder uvm_default_table_printer uvm_default_tree_printer UVM_HDL_MAX_WIDTH "
			"uvm_top value verbosity "
			//! all uvm constant
			"BODY BEGIN_REQ BEGIN_RESP CREATED END_REQ END_RESP ENDED FINISHED POST_BODY POST_START PRE_START PRE_BODY "
			"SEQ_ARB_FIFO SEQ_ARB_WEIGHTED SEQ_ARB_RANDOM SEQ_ARB_STRICT_FIFO SEQ_ARB_STRICT_RANDOM SEQ_ARB_USER STOPPED "
			"UNINITIALIZED_PHASE UVM_ALL_DROPPED UVM_BACKDOOR UVM_BIG_ENDIAN UVM_BIG_FIFO UVM_BIN UVM_CALL_HOOK UVM_CHECK "
			"UVM_COMPLETED UVM_COUNT UVM_CVR_ADDR_MAP UVM_CVR_ALL UVM_CVR_FIELD_VALS UVM_CVR_REG_BITS UVM_DEC UVM_DEEP "
			"UVM_DEFAULT_PATH UVM_DISPLAY UVM_DO_ALL_REG_MEM_TESTS UVM_DO_MEM_ACCESS UVM_DO_MEM_WALK UVM_DO_REG_ACCESS "
			"UVM_DO_REG_BIT_BASH UVM_DO_REG_HW_RESET UVM_DO_SHARED_ACCESS UVM_DROPPED UVM_ENUM UVM_EQ UVM_ERROR UVM_EXIT "
			"UVM_EXPORT UVM_FATAL UVM_FIELD UVM_FORCED_STOP UVM_FRONTDOOR UVM_FULL UVM_GT UVM_GTE UVM_HAS_X UVM_HEX UVM_HIER "
			"UVM_HIGH UVM_IMPLEMENTATION UVM_INFO UVM_IS_OK UVM_LITTLE_ENDIAN UVM_LITTLE_FIFO UVM_LOG UVM_LOW UVM_LT "
			"UVM_LTE UVM_MEDIUM UVM_MEM UVM_NE UVM_NO_ACTION UVM_NO_CHECK UVM_NO_COVERAGE UVM_NO_ENDIAN UVM_NO_HIER UVM_NONE "
			"UVM_NOT_OK UVM_OCT UVM_PHASE_CLEANUP UVM_PHASE_DOMAIN UVM_PHASE_DONE UVM_PHASE_DORMANT UVM_PHASE_ENDED UVM_PHASE_EXECUTING "
			"UVM_PHASE_IMP UVM_PHASE_NODE UVM_PHASE_READY_TO_END UVM_PHASE_SCHEDULE UVM_PHASE_SCHEDULED UVM_PHASE_STARTED "
			"UVM_PHASE_SYNCING UVM_PHASE_TERMINAL UVM_PORT UVM_PREDICT UVM_PREDICT_DIRECT UVM_PREDICT_READ UVM_PREDICT_WRITE "
			"UVM_RAISED UVM_READ UVM_REFERENCE UVM_REG UVM_RERUN UVM_SEQ_LIB_ITEM UVM_SEQ_LIB_RAND UVM_SEQ_LIB_RANDC "
			"UVM_SEQ_LIB_USER UVM_SHALLOW UVM_SKIPPED UVM_STOP UVM_STRING UVM_TIME UVM_TLM_ACCEPTED UVM_TLM_ADDRESS_ERROR_RESPONSE "
			"UVM_TLM_BURST_ERROR_RESPONSE UVM_TLM_BYTE_ENABLE_ERROR_RESPONSE UVM_TLM_COMMAND_ERROR_RESPONSE UVM_TLM_COMPLETED "
			"UVM_TLM_GENERIC_ERROR_RESPONSE UVM_TLM_IGNORE_COMMAND UVM_TLM_INCOMPLETE_RESPONSE UVM_TLM_OK_RESPONSE UVM_TLM_READ_COMMAND "
			"UVM_TLM_UPDATED UVM_TLM_WRITE_COMMAND UVM_UNSIGNED UVM_WARNING UVM_WRITE "
			//! all uvm enum
			"UVM_MAX_STREAMBITS UVM_PACKER_MAX_BYTES UVM_STREAMBITS UVM_DEFAULT_TIMEOUT UVM_UNSIGNED UVM_UNFORMAT2 "
			"UVM_UNFORMAT4 UVM_NORADIX UVM_RADIX UVM_DEFAULT_POLICY UVM_DEEP UVM_SHALLOW UVM_REFERENCE UVM_PASSIVE "
			"UVM_ACTIVE UVM_MACRO_NUMFLAGS UVM_DEFAULT UVM_ALL_ON UVM_FLAGS_ON UVM_FLAGS_OFF UVM_COPY "
			"UVM_NOCOPY UVM_COMPARE UVM_NOCOMPARE UVM_PRINT UVM_NOPRINT UVM_RECORD UVM_NORECORD UVM_PACK UVM_NOPACK "
			"UVM_PHYSICAL UVM_ABSTRACT UVM_READONLY UVM_NODEFPRINT UVM_MACRO_EXTRAS UVM_FLAGS UVM_UNPACK UVM_CHECK_FIELDS "
			"UVM_END_DATA_EXTRA UVM_START_FUNCS UVM_SET UVM_SETINT UVM_SETOBJ UVM_SETSTR UVM_END_FUNCS UVM_DEBUG "
			"UVM_PHASE_GLOBAL UVM_PHASE_JUMPING";

	return 0;
}


// Return the string of characters that comprise a word.
const char *QsciLexerSystemVerilog::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$`+";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerSystemVerilog::defaultColor(int style) const
{
    switch (style)
    {
	
	/* Black */
    case Default:
        return QColor(0x00,0x00,0x00);
	case Operator:
        return QColor(0x00,0x00,0x00);
	case UnclosedString:
        return QColor(0x00,0x00,0x00);
	case Identifier:
        return QColor(0x00,0x00,0x00);
	   
	/* Forest Green */
    case Comment:
		return QColor(0x22,0x8B,0x22);
    case CommentLine:
        return QColor(0x22,0x8B,0x22);
    case CommentBang:
        return QColor(0x22,0x8B,0x22);

	// Dark Orange
    case Number:
		return QColor(0xFF,0x8C,0x00);
		
	/* Blue */
	case Keyword:
        return QColor(0x00,0x00,0xFF);
		
	/* Purple */
    case KeywordSet2:
        return QColor(0x7D,0x26,0xCD);
	
	/* Red */
	case KeywordUvmClass:
        return QColor(0xFF,0x00,0x00);
	case KeywordUvmMethod:
        return QColor(0xFF,0x00,0x00);
	
	/* Cyan */
	case KeywordUvmMacro:
        return QColor(0x00,0xCD,0xCD);	
	
	/*case KeywordUvmVariable:
        return QColor(0x00,0xCD,0xCD);		
	case KeywordUvmContant:
        return QColor(0x00,0xCD,0xCD);		
	case KeywordUvmEnum:
        return QColor(0x00,0xCD,0xCD);		
	case KeywordUvmPort:
        return QColor(0x00,0xCD,0xCD);*/
		
	case Preprocessor:
        return QColor(0x00,0xCD,0xCD);	
		
	// Grey
    case String:
        return QColor(0xB5,0xB5,0xB5);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerSystemVerilog::defaultEolFill(int style) const
{
    if (style == CommentBang || style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerSystemVerilog::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case CommentBang:
    case Keyword:
	
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the user name of a style.
QString QsciLexerSystemVerilog::description(int style) const
{   
   switch (style)
    {
		case Default:
			return tr("Default");

		case Comment:
			return tr("Comment");

		case CommentLine:
			return tr("Line comment");

		case CommentBang:
			return tr("Bang comment");

		case Number:
			return tr("Number");

		case Keyword:
			return tr("Primary keywords and identifiers");

		case String:
			return tr("String");

		case KeywordSet2:
			return tr("Secondary keywords and identifiers");

		case KeywordUvmClass:
			return tr("UVM class");
			
		case KeywordUvmMethod:
			return tr("UVM method");
			
		case KeywordUvmMacro:
			return tr("UVM macro");
		
		case Preprocessor:
			return tr("Preprocessor block");

		case Operator:
			return tr("Operator");

		case Identifier:
			return tr("Identifier");

		case UnclosedString:
			return tr("Unclosed string");
	}

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerSystemVerilog::defaultPaper(int style) const
{
    if (style == CommentBang)
        return QColor(0xe0,0xf0,0xff);

    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerSystemVerilog::refreshProperties()
{
    setAtElseProp();
    setCommentProp();
    setCompactProp();
    setPreprocProp();
}


// Read properties from the settings.
bool QsciLexerSystemVerilog::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();
    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_preproc = qs.value(prefix + "foldpreprocessor", false).toBool();
    fold_atmodule = qs.value(prefix + "foldsystemverilogflags", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerSystemVerilog::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpreprocessor", fold_preproc);
    qs.setValue(prefix + "foldsystemverilogflags", fold_atmodule);

    return rc;
}

// Return true if else can be folded.
bool QsciLexerSystemVerilog::foldAtElse() const
{
    return fold_atelse;
}


// Set if else can be folded.
void QsciLexerSystemVerilog::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerSystemVerilog::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Return true if comments can be folded.
bool QsciLexerSystemVerilog::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerSystemVerilog::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerSystemVerilog::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerSystemVerilog::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerSystemVerilog::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerSystemVerilog::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool QsciLexerSystemVerilog::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void QsciLexerSystemVerilog::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void QsciLexerSystemVerilog::setPreprocProp()
{
    emit propertyChanged("fold.preprocessor",(fold_preproc ? "1" : "0"));
}


// Return true if modules can be folded.
bool QsciLexerSystemVerilog::foldAtModule() const
{
    return fold_atmodule;
}


// Set if modules can be folded.
void QsciLexerSystemVerilog::setFoldAtModule(bool fold)
{
    fold_atmodule = fold;

    setAtModuleProp();
}


// Set the "fold.systemverilog.flags" property.
void QsciLexerSystemVerilog::setAtModuleProp()
{
    emit propertyChanged("fold.systemverilog.flags",(fold_atmodule ? "1" : "0"));
}
