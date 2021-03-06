/****************************************************************************
 *	Copyright 2012 HES-SO HEIG-VD
 *	Copyright 2011 HES-SO Valais Wallis
 *	Copyright 2007-2010 Mentor Graphics Corporation
 *	Copyright 2007-2010 Cadence Design Systems, Inc.
 *	Copyright 2010 Synopsys, Inc.
 *	All Rights Reserved Worldwide
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 ****************************************************************************
 * This file has been generated by UVE version $@uve_version@$
 * Date of generation: $@date@$
 *
 * Project : $@projectname@$
 * File : $@filename@$
 * Version : 1.0
 ****************************************************************************
 * Description :
 * Here are described high-level sequences.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

/*---------------------------------------------------------------------------
 * @TODO : Implement sequences. Only one example sequence is presented
 *         below. They can hinerit from:
 $@copy * - %vip%_base_sequence (%vip%_master_seq_lib.sv)
 *
 *---- Example -------------------------------------------------------------*/

$@ copy `include "%vip%_master_seq_lib.sv"

/**********************************************************
 * A Master Sequence
 **********************************************************/

class a_master_seq extends $@vip_name@$_base_sequence;

    // Constructor
    function new(string name="a_master_seq");
        super.new(name);
    endfunction : new

    // Provide implementations of virtual methods such as get_type_name and create
    `uvm_object_utils(a_master_seq)

    // Use a sequence from gvip_master_seq_lib
    $@ copy %vip%_a_req %vip%_a_req0;

    // Define random signal for transaction

    // Define constraint for this signals

    // Body
    virtual task body();

        // Diplay start of sequence
        `uvm_info(get_type_name(),
                  $psprintf("%s starting...",
                  get_sequence_path()), UVM_MEDIUM);

        // Execute the sequence
        $@ copy `uvm_do(%vip%_a_req0)

        // Use the macro `uvm_do_with if you wand to specify interface signals

    endtask : body

endclass : a_master_seq

/*--------------------------------------------------------------------------*/

`endif /* $@FILENAME@$ */
