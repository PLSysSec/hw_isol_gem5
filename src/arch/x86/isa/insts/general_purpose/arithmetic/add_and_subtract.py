# Copyright (c) 2007 The Hewlett-Packard Development Company
# All rights reserved.
#
# The license below extends only to copyright in the software and shall
# not be construed as granting a license to any other intellectual
# property including but not limited to intellectual property relating
# to a hardware implementation of the functionality of the software
# licensed hereunder.  You may use the software subject to the license
# terms below provided that you ensure that this notice is replicated
# unmodified and in its entirety in all distributions of the software,
# modified or unmodified, in source code or in binary form.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

microcode = '''
def macroop ADD_R_R
{
    add reg, reg, regm, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop LOAD_RANGE
{
    limm t0, 0
    ld t1, seg, [1, t0, rax], dataSize=8
    ld t2, seg, [1, t0, rax], 8, dataSize=8
    ld t3, seg, [1, t0, rax], 16, dataSize=8

    rdval t6, "InstRegIndex(MISCREG_SANDBOX_BASE_1)"
    sub t4, t2, t6, flags=(SF,)
    br label("error"), flags=(CSF,)
    add t4, t2, t3
    rdval t7, "InstRegIndex(MISCREG_SANDBOX_SIZE_1)"
    add t5, t6, t7
    sub t4, t5, t4, flags=(SF,)
    br label("error"), flags=(CSF,)

    wrval "InstRegIndex(MISCREG_SANDBOX_ID)", t1
    wrval "InstRegIndex(MISCREG_SANDBOX_BASE_1)", t2
    wrval "InstRegIndex(MISCREG_SANDBOX_SIZE_1)", t3

    br label("end")

error:
    fault "std::make_shared<BoundsCheck>()",

end:
    fault "NoFault"

};

def macroop SAND_ENABLE
{
    limm t1, 1
    wrval "InstRegIndex(MISCREG_SANDBOX_EN)", t1

};

def macroop SAND_DISABLE
{
    limm t1, 0
    wrval "InstRegIndex(MISCREG_SANDBOX_EN)", t1
};

def macroop HFI_GET_VERSION
{
    limm rax, 2
};

def macroop HFI_GET_LINEAR_RANGE_COUNT
{
    limm rax, 4
};

def macroop HFI_ENTER_SANDBOX
{
    # if (reg_hfi_curr.inside_sandbox) { SIGSEV; }
    rdval t1, "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)"
    and t1, t1, t1, flags=(ZF,)
    br label("hfi_enter_sandbox_continue"), flags=(CZF,)
    fault "std::make_shared<BoundsCheck>()",

hfi_enter_sandbox_continue:
    limm t1, 0
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_UPPER_BOUND)", t7

    limm t1, 32
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_UPPER_BOUND)", t7

    limm t1, 64
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_UPPER_BOUND)", t7

    limm t1, 96
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_UPPER_BOUND)", t7

    limm t1, 128
    limm t2, 0
    limm t3, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 8, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_MOV)", t2
    wrval "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_STACK)", t3
    wrval "InstRegIndex(MISCREG_HFI_EXIT_SANDBOX_HANDLER)", t4

    limm t1, 1
    .serialize_after
    wrval "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)", t1
};

def macroop HFI_EXIT_SANDBOX
{
    # if (!reg_hfi_curr.inside_sandbox) { SIGSEV; }
    rdval t1, "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)"
    and t1, t1, t1, flags=(ZF,)
    br label("hfi_exit_sandbox_continue"), flags=(nCZF,)
    fault "std::make_shared<BoundsCheck>()",

hfi_exit_sandbox_continue:
    wrval "InstRegIndex(MISCREG_HFI_EXIT_REASON)", t0
    wrval "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)", t0

    # if (exit_sandbox_handler) { jmp exit_sandbox_handler; }
    rdval t1, "InstRegIndex(MISCREG_HFI_EXIT_SANDBOX_HANDLER)"
    and t2, t1, t1, flags=(ZF,)
    .serialize_after
    br label("hfi_exit_sandbox_nohandler"), flags=(CZF,)

    # Make the default data size of jumps 64 bits in 64 bit mode
    .adjust_env oszIn64Override
    .control_indirect
    wripi t1, 0

hfi_exit_sandbox_nohandler:
    limm t0, 0
};

# TODO: below instructions should check for ring 0

def macroop HFI_SAVE_THREAD_CONTEXT
{
    limm t1, 0
    rdval t2, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_READABLE)"
    rdval t3, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_WRITEABLE)"
    rdval t4, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_EXECUTABLE)"
    rdval t5, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_BASE_ADDRESS)"
    rdval t6, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_LOWER_BOUND)"
    rdval t7, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_UPPER_BOUND)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 1, dataSize=1
    st t4, seg, [1, t1, rax], 2, dataSize=1
    st t5, seg, [1, t1, rax], 8, dataSize=8
    st t6, seg, [1, t1, rax], 16, dataSize=8
    st t7, seg, [1, t1, rax], 24, dataSize=8

    limm t1, 32
    rdval t2, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_READABLE)"
    rdval t3, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_WRITEABLE)"
    rdval t4, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_EXECUTABLE)"
    rdval t5, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_BASE_ADDRESS)"
    rdval t6, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_LOWER_BOUND)"
    rdval t7, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_UPPER_BOUND)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 1, dataSize=1
    st t4, seg, [1, t1, rax], 2, dataSize=1
    st t5, seg, [1, t1, rax], 8, dataSize=8
    st t6, seg, [1, t1, rax], 16, dataSize=8
    st t7, seg, [1, t1, rax], 24, dataSize=8

    limm t1, 64
    rdval t2, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_READABLE)"
    rdval t3, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_WRITEABLE)"
    rdval t4, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_EXECUTABLE)"
    rdval t5, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_BASE_ADDRESS)"
    rdval t6, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_LOWER_BOUND)"
    rdval t7, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_UPPER_BOUND)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 1, dataSize=1
    st t4, seg, [1, t1, rax], 2, dataSize=1
    st t5, seg, [1, t1, rax], 8, dataSize=8
    st t6, seg, [1, t1, rax], 16, dataSize=8
    st t7, seg, [1, t1, rax], 24, dataSize=8

    limm t1, 96
    rdval t2, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_READABLE)"
    rdval t3, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_WRITEABLE)"
    rdval t4, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_EXECUTABLE)"
    rdval t5, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_BASE_ADDRESS)"
    rdval t6, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_LOWER_BOUND)"
    rdval t7, "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_UPPER_BOUND)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 1, dataSize=1
    st t4, seg, [1, t1, rax], 2, dataSize=1
    st t5, seg, [1, t1, rax], 8, dataSize=8
    st t6, seg, [1, t1, rax], 16, dataSize=8
    st t7, seg, [1, t1, rax], 24, dataSize=8

    limm t1, 128
    rdval t2, "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_MOV)"
    rdval t3, "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_STACK)"
    rdval t4, "InstRegIndex(MISCREG_HFI_EXIT_SANDBOX_HANDLER)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 1, dataSize=1
    st t4, seg, [1, t1, rax], 8, dataSize=8

    limm t1, 144
    rdval t2, "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)"
    rdval t3, "InstRegIndex(MISCREG_HFI_EXIT_REASON)"
    st t2, seg, [1, t1, rax], dataSize=1
    st t3, seg, [1, t1, rax], 4, dataSize=4
};

def macroop HFI_LOAD_THREAD_CONTEXT
{
    limm t1, 0
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_1_UPPER_BOUND)", t7

    limm t1, 32
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_2_UPPER_BOUND)", t7

    limm t1, 64
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_3_UPPER_BOUND)", t7

    limm t1, 96
    limm t2, 0
    limm t3, 0
    limm t4, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 2, dataSize=1
    ld t5, seg, [1, t1, rax], 8, dataSize=8
    ld t6, seg, [1, t1, rax], 16, dataSize=8
    ld t7, seg, [1, t1, rax], 24, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_READABLE)", t2
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_WRITEABLE)", t3
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_EXECUTABLE)", t4
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_BASE_ADDRESS)", t5
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_LOWER_BOUND)", t6
    wrval "InstRegIndex(MISCREG_HFI_LINEAR_RANGE_4_UPPER_BOUND)", t7

    limm t1, 128
    limm t2, 0
    limm t3, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 1, dataSize=1
    ld t4, seg, [1, t1, rax], 8, dataSize=8
    wrval "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_MOV)", t2
    wrval "InstRegIndex(MISCREG_HFI_DISALLOW_UNRESTRICTED_STACK)", t3
    wrval "InstRegIndex(MISCREG_HFI_EXIT_SANDBOX_HANDLER)", t4

    limm t1, 144
    limm t2, 0
    limm t3, 0
    ld t2, seg, [1, t1, rax], dataSize=1
    ld t3, seg, [1, t1, rax], 4, dataSize=4
    wrval "InstRegIndex(MISCREG_HFI_INSIDE_SANDBOX)", t2
    wrval "InstRegIndex(MISCREG_HFI_EXIT_REASON)", t3
};

def macroop HFI_GET_EXIT_REASON
{
    rdval rax, "InstRegIndex(MISCREG_HFI_EXIT_REASON)"
};

def macroop ADD_R_I
{
    limm t1, imm
    add reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop ADD_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    add t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop ADD_P_I
{
    rdip t7
    limm t2, imm
    ldst t1, seg, riprel, disp
    add t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop ADD_LOCKED_M_I
{
    limm t2, imm
    mfence
    ldstl t1, seg, sib, disp
    add t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop ADD_LOCKED_P_I
{
    rdip t7
    limm t2, imm
    mfence
    ldstl t1, seg, riprel, disp
    add t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop ADD_M_R
{
    ldst t1, seg, sib, disp
    add t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop ADD_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    add t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop ADD_LOCKED_M_R
{
    mfence
    ldstl t1, seg, sib, disp
    add t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop ADD_LOCKED_P_R
{
    rdip t7
    mfence
    ldstl t1, seg, riprel, disp
    add t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop ADD_R_M
{
    ld t1, seg, sib, disp
    add reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop ADD_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    add reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SUB_R_R
{
    sub reg, reg, regm, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SUB_R_I
{
    limm t1, imm
    sub reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SUB_R_M
{
    ld t1, seg, sib, disp
    sub reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SUB_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    sub reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SUB_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    sub t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop SUB_P_I
{
    rdip t7
    limm t2, imm
    ldst t1, seg, riprel, disp
    sub t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop SUB_LOCKED_M_I
{
    limm t2, imm
    mfence
    ldstl t1, seg, sib, disp
    sub t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop SUB_LOCKED_P_I
{
    rdip t7
    limm t2, imm
    mfence
    ldstl t1, seg, riprel, disp
    sub t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop SUB_M_R
{
    ldst t1, seg, sib, disp
    sub t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop SUB_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    sub t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop SUB_LOCKED_M_R
{
    mfence
    ldstl t1, seg, sib, disp
    sub t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop SUB_LOCKED_P_R
{
    rdip t7
    mfence
    ldstl t1, seg, riprel, disp
    sub t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop ADC_R_R
{
    adc reg, reg, regm, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop ADC_R_I
{
    limm t1, imm
    adc reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop ADC_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    adc t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop ADC_P_I
{
    rdip t7
    limm t2, imm
    ldst t1, seg, riprel, disp
    adc t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop ADC_LOCKED_M_I
{
    limm t2, imm
    mfence
    ldstl t1, seg, sib, disp
    adc t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop ADC_LOCKED_P_I
{
    rdip t7
    limm t2, imm
    mfence
    ldstl t1, seg, riprel, disp
    adc t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop ADC_M_R
{
    ldst t1, seg, sib, disp
    adc t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop ADC_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    adc t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop ADC_LOCKED_M_R
{
    mfence
    ldstl t1, seg, sib, disp
    adc t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop ADC_LOCKED_P_R
{
    rdip t7
    mfence
    ldstl t1, seg, riprel, disp
    adc t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop ADC_R_M
{
    ld t1, seg, sib, disp
    adc reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop ADC_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    adc reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SBB_R_R
{
    sbb reg, reg, regm, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SBB_R_I
{
    limm t1, imm
    sbb reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SBB_R_M
{
    ld t1, seg, sib, disp
    sbb reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SBB_R_P
{
    rdip t7
    ld t1, seg, riprel, disp
    sbb reg, reg, t1, flags=(OF,SF,ZF,AF,PF,CF)
};

def macroop SBB_M_I
{
    limm t2, imm
    ldst t1, seg, sib, disp
    sbb t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop SBB_P_I
{
    rdip t7
    limm t2, imm
    ldst t1, seg, riprel, disp
    sbb t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop SBB_LOCKED_M_I
{
    limm t2, imm
    mfence
    ldstl t1, seg, sib, disp
    sbb t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop SBB_LOCKED_P_I
{
    rdip t7
    limm t2, imm
    mfence
    ldstl t1, seg, riprel, disp
    sbb t1, t1, t2, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop SBB_M_R
{
    ldst t1, seg, sib, disp
    sbb t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, sib, disp
};

def macroop SBB_P_R
{
    rdip t7
    ldst t1, seg, riprel, disp
    sbb t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    st t1, seg, riprel, disp
};

def macroop SBB_LOCKED_M_R
{
    mfence
    ldstl t1, seg, sib, disp
    sbb t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, sib, disp
    mfence
};

def macroop SBB_LOCKED_P_R
{
    rdip t7
    mfence
    ldstl t1, seg, riprel, disp
    sbb t1, t1, reg, flags=(OF,SF,ZF,AF,PF,CF)
    stul t1, seg, riprel, disp
    mfence
};

def macroop NEG_R
{
    sub reg, t0, reg, flags=(CF,OF,SF,ZF,AF,PF)
};

def macroop NEG_M
{
    ldst t1, seg, sib, disp
    sub t1, t0, t1, flags=(CF,OF,SF,ZF,AF,PF)
    st t1, seg, sib, disp
};

def macroop NEG_P
{
    rdip t7
    ldst t1, seg, riprel, disp
    sub t1, t0, t1, flags=(CF,OF,SF,ZF,AF,PF)
    st t1, seg, riprel, disp
};

def macroop NEG_LOCKED_M
{
    mfence
    ldstl t1, seg, sib, disp
    sub t1, t0, t1, flags=(CF,OF,SF,ZF,AF,PF)
    stul t1, seg, sib, disp
    mfence
};

def macroop NEG_LOCKED_P
{
    rdip t7
    mfence
    ldstl t1, seg, riprel, disp
    sub t1, t0, t1, flags=(CF,OF,SF,ZF,AF,PF)
    stul t1, seg, riprel, disp
    mfence
};
'''
