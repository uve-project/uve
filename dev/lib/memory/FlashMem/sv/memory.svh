/*******************************************************************************
 * -----------------------------------------------------------------------------
 * Copyright 2009 Paradigm Works, Inc., Andover MA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *------------------------------------------------------------------------------
*/
/***************************************************************************
 *
 * Author:      $Author:$
 * File:        $File:$
 * Revision:    $Revision:$
 * Date:        $Date:$
 *
 *******************************************************************************
 *
 * memory interface files
 *
 *******************************************************************************
 */
`ifndef MEMORY_SVH
`define MEMORY_SVH


`include "memory_transfer.sv"

`include "memory_if_wrapper.svh"

`include "memory_bus_monitor.svh"
`include "memory_master_driver.svh"
`include "memory_master_monitor.svh"
`include "memory_master_sequencer.svh"
`include "memory_master_agent.svh"

`include "memory_slave_driver.svh"
`include "memory_slave_monitor.svh"
`include "memory_slave_sequencer.svh"
`include "memory_slave_agent.svh"

`include "memory_env.svh"

`include "memory_bus_monitor.sv"
`include "memory_master_driver.sv"
`include "memory_master_monitor.sv"
`include "memory_master_sequencer.sv"
`include "memory_master_agent.sv"
`include "memory_master_seq_lib.sv"

`include "memory_slave_driver.sv"
`include "memory_slave_monitor.sv"
`include "memory_slave_sequencer.sv"
`include "memory_slave_agent.sv"
`include "memory_slave_seq_lib.sv"

`include "memory_env.sv"

`endif //  `ifndef MEMORY_SVH
