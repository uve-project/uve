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
 * memory agent interface
 *
 *******************************************************************************
 */
`ifndef MEMORY_IF_SV
`define MEMORY_IF_SV

interface memory_if (input clk);

  // Control flags
  bit                has_checks = 1;
  bit                has_coverage = 1;

  //-- guo -- add<
  // Parameters
  parameter int unsigned NCHIP = 3; //put into config
  //-- guo -- add>


  // Actual Signals
  // USER: Add interface signals
  //guo add<
  // flash inouts
  wire[(NCHIP*8)-1:0]   DQ0;    // IO Port 0:             for source synchronous
  wire[NCHIP-1:0]       DQS0;   // data strobe:           for source synchronous
  wire[(NCHIP*8)-1:0]   IO0;    // IO Port 0:                   for asynchronous
  // flash inputs
  logic                 RE_n;   // not read enable:             for asynchronous
  logic                 WR_n;   // not (write/read direction) = read not write
  logic                 CE_n;   // not chip enable
  logic                 CLE;    // command latch enable
  logic                 ALE;    // address latch enable
  logic                 WE_n;   // not write enable:            for asynchronous
  logic                 CLK_t;  // clock:                 for source synchronous
  logic                 WP_n;   // not write protect
  // flash outputs
  logic                 RB_n;   // not (ready not busy) = busy not ready
  // local signals
  logic[(NCHIP*8)-1:0]  IO0_drive = 'z;    // IO Port 0: logic to drive wire
  assign IO0 = IO0_drive;
  //guo add>
  

  clocking cb @(posedge clk);
   // USER: Add clocking block detail
    //guo add<
    //input-output as seen by the TB
    //input (sample) skew, #1step = just before the clock
    default input   #1step;
    //output (drive) skew, #1 = one time unit after the clock
    default output  #1;
    input RE_n, WR_n, CE_n, CLE, ALE, WE_n, WP_n, CLK_t;
    inout DQ0, DQS0, IO0;
    //TODO: change once driver implemented correctly
    output RB_n;
//    input RB_n; 
    //guo add>
  endclocking : cb

// modport DUT (
//   // USER: Add dut I/O
//   //guo add<
// 	inout  DQ0,
// 	output  RE_n, WR_n, CE_n, CLE, ALE, WE_n, WP_n, CLK_t,
// 	input   RB_n
//   //guo add>
// );

//  modport TB (
//    //guo add<
//    inout   DQ0,
//    input   RE_n, WR_n, CE_n, CLE, ALE, WE_n, WP_n, CLK_t,
//    output  RB_n
//    //guo add>
//  );

  // Coverage and assertions to be implemented here.
  // USER: Add assertions/coverage here

endinterface : memory_if

`endif // MEMORY_IF_SV
