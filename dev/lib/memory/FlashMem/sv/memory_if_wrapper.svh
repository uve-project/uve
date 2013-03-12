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
 * memory  if wrapper header
 *
 *******************************************************************************
 */
`ifndef MEMORY_IF_WRAPPER_SVH
`define MEMORY_IF_WRAPPER_SVH

class memory_if_wrapper extends uvm_object;
 
   virtual memory_if intf;
  
   function new(string name, virtual memory_if _intf);
     intf = _intf;
   endfunction : new
  
endclass : memory_if_wrapper

`endif // MEMORY_IF_WRAPPER_SVH
