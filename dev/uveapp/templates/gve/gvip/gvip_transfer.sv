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
 * Transaction represent the input to the DUV.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

/*----------------------------------------------------------------------------
 * @TODO : Maybe define enum for transaction signals
 *---- Example --------------------------------------------------------------*/
/* 
typedef enum {
        NOP,
        READ,
        WRITE
} $@class_name@$_rw_enum;
*/

/*---------------------------------------------------------------------------*/

class $@ClassName@$ extends uvm_sequence_item;

/*----------------------------------------------------------------------------
 * @TODO : Define transaction signals
 *---- Example --------------------------------------------------------------*/
 /*
        rand bit [15:0]                         addr;
        rand bit [7:0]                          data[];
        rand $@class_name@$_rw_enum             read_write;
        string                                  master = "";    // Indicate which master is querying
        string                                  slave = "";     // Indicate which master is responding
*/
        // DUT RTL signals
        $@ transfer_variables

/*---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * @TODO : Define constraints for transaction signals
 *---- Example --------------------------------------------------------------*/
        // Read Write values constraint
/*
        constraint c_read_write {
                read_write inside { READ, WRITE };
        }
*/

/*---------------------------------------------------------------------------*/


        // Provide implementations of virtual methods such as get_type_name and create
        `uvm_object_utils_begin($@ClassName@$)
/*----------------------------------------------------------------------------
 * @TODO : Use macros to implemente virtual methods for transaction signals
 *---- Example --------------------------------------------------------------*/
/*
        `uvm_field_int		(addr,				UVM_DEFAULT)
        `uvm_field_array_int    (data,				UVM_DEFAULT)
        `uvm_field_enum		($@class_name@$_rw_enum, read_write, UVM_DEFAULT)
        `uvm_field_string	(master,			UVM_DEFAULT|UVM_NOCOMPARE)
        `uvm_field_string	(slave,				UVM_DEFAULT|UVM_NOCOMPARE)
*/
        
        // DUT RTL signals
        $@ transfer_var_macro

/*---------------------------------------------------------------------------*/
        `uvm_object_utils_end

        // Constructor
        function new(string name = "$@class_name@$_inst");
                super.new(name);
        endfunction

endclass : $@ClassName@$

`endif /* $@FILENAME@$ */
