/*
 * Copyright (c) 2010-2011 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2004-2006 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CPU_O3_DYN_INST_IMPL_HH__
#define __CPU_O3_DYN_INST_IMPL_HH__

#include "arch/x86/faults.hh"
#include "cpu/o3/dyn_inst.hh"
#include "debug/O3PipeView.hh"
#include "debug/HFI.hh"

template <class Impl>
BaseO3DynInst<Impl>::BaseO3DynInst(const StaticInstPtr &staticInst,
                                   const StaticInstPtr &macroop,
                                   TheISA::PCState pc, TheISA::PCState predPC,
                                   InstSeqNum seq_num, O3CPU *cpu)
    : BaseDynInst<Impl>(staticInst, macroop, pc, predPC, seq_num, cpu)
{
    initVars();
}

template <class Impl>
BaseO3DynInst<Impl>::BaseO3DynInst(const StaticInstPtr &_staticInst,
                                   const StaticInstPtr &_macroop)
    : BaseDynInst<Impl>(_staticInst, _macroop)
{
    initVars();
}

template <class Impl>BaseO3DynInst<Impl>::~BaseO3DynInst()
{
#if TRACING_ON
    if (DTRACE(O3PipeView)) {
        Tick fetch = this->fetchTick;
        // fetchTick can be -1 if the instruction fetched outside the trace window.
        if (fetch != -1) {
            Tick val;
            // Print info needed by the pipeline activity viewer.
            DPRINTFR(O3PipeView, "O3PipeView:fetch:%llu:0x%08llx:%d:%llu:%s\n",
                     fetch,
                     this->instAddr(),
                     this->microPC(),
                     this->seqNum,
                     this->staticInst->disassemble(this->instAddr()));

            val = (this->decodeTick == -1) ? 0 : fetch + this->decodeTick;
            DPRINTFR(O3PipeView, "O3PipeView:decode:%llu\n", val);
            val = (this->renameTick == -1) ? 0 : fetch + this->renameTick;
            DPRINTFR(O3PipeView, "O3PipeView:rename:%llu\n", val);
            val = (this->dispatchTick == -1) ? 0 : fetch + this->dispatchTick;
            DPRINTFR(O3PipeView, "O3PipeView:dispatch:%llu\n", val);
            val = (this->issueTick == -1) ? 0 : fetch + this->issueTick;
            DPRINTFR(O3PipeView, "O3PipeView:issue:%llu\n", val);
            val = (this->completeTick == -1) ? 0 : fetch + this->completeTick;
            DPRINTFR(O3PipeView, "O3PipeView:complete:%llu\n", val);
            val = (this->commitTick == -1) ? 0 : fetch + this->commitTick;

            Tick valS = (this->storeTick == -1) ? 0 : fetch + this->storeTick;
            DPRINTFR(O3PipeView, "O3PipeView:retire:%llu:store:%llu\n", val, valS);
        }
    }
#endif
};


template <class Impl>
void
BaseO3DynInst<Impl>::initVars()
{
    this->_readySrcRegIdx.reset();

    _numDestMiscRegs = 0;

#if TRACING_ON
    // Value -1 indicates that particular phase
    // hasn't happened (yet).
    fetchTick = -1;
    decodeTick = -1;
    renameTick = -1;
    dispatchTick = -1;
    issueTick = -1;
    completeTick = -1;
    commitTick = -1;
    storeTick = -1;
#endif
}

template <class Impl>
Fault
BaseO3DynInst<Impl>::execute()
{
    // @todo: Pretty convoluted way to avoid squashing from happening
    // when using the TC during an instruction's execution
    // (specifically for instructions that have side-effects that use
    // the TC).  Fix this.
    bool no_squash_from_TC = this->thread->noSquashFromTC;
    this->thread->noSquashFromTC = true;

    this->fault = this->staticInst->execute(this, this->traceData);

    this->thread->noSquashFromTC = no_squash_from_TC;

    return this->fault;
}

template <class Impl>
Fault
BaseO3DynInst<Impl>::initiateAcc()
{
    // @todo: Pretty convoluted way to avoid squashing from happening
    // when using the TC during an instruction's execution
    // (specifically for instructions that have side-effects that use
    // the TC).  Fix this.
    bool no_squash_from_TC = this->thread->noSquashFromTC;
    this->thread->noSquashFromTC = true;

    this->fault = this->staticInst->initiateAcc(this, this->traceData);

    this->thread->noSquashFromTC = no_squash_from_TC;

    return this->fault;
}

template <class Impl>
Fault
BaseO3DynInst<Impl>::completeAcc(PacketPtr pkt)
{
    // @todo: Pretty convoluted way to avoid squashing from happening
    // when using the TC during an instruction's execution
    // (specifically for instructions that have side-effects that use
    // the TC).  Fix this.
    bool no_squash_from_TC = this->thread->noSquashFromTC;
    this->thread->noSquashFromTC = true;

    if (this->cpu->checker) {
        if (this->isStoreConditional()) {
            this->reqToVerify->setExtraData(pkt->req->getExtraData());
        }
    }

    this->fault = this->staticInst->completeAcc(pkt, this, this->traceData);

    this->thread->noSquashFromTC = no_squash_from_TC;

    return this->fault;
}


template <class Impl>
void
BaseO3DynInst<Impl>::printHFIMetadata() {
    using namespace TheISA;

    DPRINTF(HFI, "HFI sandbox bounds metadata!\n");
    DPRINTF(HFI, "HFI_LINEAR_RANGE_1_READABLE: %" PRIu64 "\n",     (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_1_READABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_1_WRITEABLE: %" PRIu64 "\n",    (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_1_WRITEABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_1_BASE_ADDRESS: %" PRIu64 "\n", (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_1_BASE_ADDRESS));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_1_LOWER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_1_LOWER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_1_UPPER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_1_UPPER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_2_READABLE: %" PRIu64 "\n",     (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_2_READABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_2_WRITEABLE: %" PRIu64 "\n",    (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_2_WRITEABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_2_BASE_ADDRESS: %" PRIu64 "\n", (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_2_BASE_ADDRESS));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_2_LOWER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_2_LOWER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_2_UPPER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_2_UPPER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_3_READABLE: %" PRIu64 "\n",     (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_3_READABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_3_WRITEABLE: %" PRIu64 "\n",    (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_3_WRITEABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_3_BASE_ADDRESS: %" PRIu64 "\n", (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_3_BASE_ADDRESS));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_3_LOWER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_3_LOWER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_3_UPPER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_3_UPPER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_4_READABLE: %" PRIu64 "\n",     (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_4_READABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_4_WRITEABLE: %" PRIu64 "\n",    (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_4_WRITEABLE));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_4_BASE_ADDRESS: %" PRIu64 "\n", (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_4_BASE_ADDRESS));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_4_LOWER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_4_LOWER_BOUND));
    DPRINTF(HFI, "HFI_LINEAR_RANGE_4_UPPER_BOUND: %" PRIu64 "\n",  (uint64_t) readMiscReg(MISCREG_HFI_LINEAR_RANGE_4_UPPER_BOUND));
    DPRINTF(HFI, "HFI_IS_TRUSTED_SANDBOX: %" PRIu64 "\n",          (uint64_t) readMiscReg(MISCREG_HFI_IS_TRUSTED_SANDBOX));
    DPRINTF(HFI, "HFI_EXIT_SANDBOX_HANDLER: %" PRIu64 "\n",        (uint64_t) readMiscReg(MISCREG_HFI_EXIT_SANDBOX_HANDLER));
    DPRINTF(HFI, "HFI_INSIDE_SANDBOX: %" PRIu64 "\n",              (uint64_t) readMiscReg(MISCREG_HFI_INSIDE_SANDBOX));
    DPRINTF(HFI, "HFI_EXIT_REASON: %" PRIu64 "\n",                 (uint64_t) readMiscReg(MISCREG_HFI_EXIT_REASON));
    DPRINTF(HFI, "HFI_EXIT_LOCATION: %" PRIu64 "\n",               (uint64_t) readMiscReg(MISCREG_HFI_EXIT_LOCATION));
}

template <class Impl>
Addr
BaseO3DynInst<Impl>::doHFIRangeCheck(Addr EA,
    TheISA::MiscRegIndex reg_base,
    TheISA::MiscRegIndex reg_lower,
    TheISA::MiscRegIndex reg_upper,
    TheISA::MiscRegIndex reg_perm,
    bool& out_found, bool& out_faulted)
{
    out_found = EA >= readMiscReg(reg_lower) && EA <= readMiscReg(reg_upper);
    Addr fullAddress = EA;
    if (out_found) {
        if(!readMiscReg(reg_perm)) {
            out_faulted = true;
        } else {
            auto base = readMiscReg(reg_base);
            if (base != 0) {
                DPRINTF(HFI, "Adding base: %lu\n", (long unsigned) base);
            }
            fullAddress += base;
        }
    }
    return fullAddress;
}


template <class Impl>
Fault
BaseO3DynInst<Impl>::checkHFI(Addr &EA, bool is_store){
    using namespace TheISA;

    bool allow_unrestricted = this->readMiscReg(MISCREG_HFI_IS_TRUSTED_SANDBOX) == 1;
    // bool is_unrestricted_mov_instruction = this->macroop->isUnrestricted();

    bool is_inside_sandbox = this->readMiscReg(MISCREG_HFI_INSIDE_SANDBOX) != 0;

    bool apply_bounds_checks = is_inside_sandbox && !allow_unrestricted;

    if (is_inside_sandbox || apply_bounds_checks) {
        DPRINTF(HFI,
            "HFI bounds check is %s necessary for %s, EA=%lx \n"
            "HFI allow_unrestricted: %d\n"
            "HFI is_inside_sandbox: %d\n"
            "HFI apply_bounds_checks: %d\n",
            apply_bounds_checks? "" : "not",
            this->macroop->getName().c_str(), EA,
            (int) allow_unrestricted,
            (int) is_inside_sandbox,
            (int) apply_bounds_checks
        );
    }

    if (!apply_bounds_checks){
        return NoFault;
    }

    DPRINTF(HFI, "checking bounds for %s, EA=%x \n", this->macroop->getName(), EA);

    printHFIMetadata();

    bool out_found = false;
    bool out_faulted = false;

    MiscRegIndex hfi_regs_base[]  = { MISCREG_HFI_LINEAR_RANGE_1_BASE_ADDRESS, MISCREG_HFI_LINEAR_RANGE_2_BASE_ADDRESS, MISCREG_HFI_LINEAR_RANGE_3_BASE_ADDRESS, MISCREG_HFI_LINEAR_RANGE_4_BASE_ADDRESS };
    MiscRegIndex hfi_regs_lower[] = { MISCREG_HFI_LINEAR_RANGE_1_LOWER_BOUND , MISCREG_HFI_LINEAR_RANGE_2_LOWER_BOUND , MISCREG_HFI_LINEAR_RANGE_3_LOWER_BOUND , MISCREG_HFI_LINEAR_RANGE_4_LOWER_BOUND  };
    MiscRegIndex hfi_regs_upper[] = { MISCREG_HFI_LINEAR_RANGE_1_UPPER_BOUND , MISCREG_HFI_LINEAR_RANGE_2_UPPER_BOUND , MISCREG_HFI_LINEAR_RANGE_3_UPPER_BOUND , MISCREG_HFI_LINEAR_RANGE_4_UPPER_BOUND  };
    MiscRegIndex hfi_regs_read[]  = { MISCREG_HFI_LINEAR_RANGE_1_READABLE    , MISCREG_HFI_LINEAR_RANGE_2_READABLE    , MISCREG_HFI_LINEAR_RANGE_3_READABLE    , MISCREG_HFI_LINEAR_RANGE_4_READABLE     };
    MiscRegIndex hfi_regs_write[] = { MISCREG_HFI_LINEAR_RANGE_1_WRITEABLE   , MISCREG_HFI_LINEAR_RANGE_2_WRITEABLE   , MISCREG_HFI_LINEAR_RANGE_3_WRITEABLE   , MISCREG_HFI_LINEAR_RANGE_4_WRITEABLE    };
    MiscRegIndex* hfi_regs_perm   = is_store? hfi_regs_write : hfi_regs_read;

    uint64_t final_effective_addr = 0;

    for (uint64_t i = 0; i < 4; i++) {
        final_effective_addr = doHFIRangeCheck(EA,
            hfi_regs_base[i], hfi_regs_lower[i], hfi_regs_upper[i], hfi_regs_perm[i],
            out_found, out_faulted);
        if (out_found) { break; }
    }

    if(!out_found || out_faulted) {
        std::cout << "SFI " << (is_store? "store" : "load") << " fault: " << EA << "!\n";

        // redo the check below for easy debugging when a debugger is attached
        for (uint64_t i = 0; i < 4; i++) {
            final_effective_addr = doHFIRangeCheck(EA,
                hfi_regs_base[i], hfi_regs_lower[i], hfi_regs_upper[i], hfi_regs_perm[i],
                out_found, out_faulted);
            if (out_found) { break; }
        }
        return std::make_shared<TheISA::HFIBoundsCheck>();
    } else {
        // update the effective address here
        EA = final_effective_addr;
    }

    return NoFault;
}

template <class Impl>
void
BaseO3DynInst<Impl>::trap(const Fault &fault)
{
    this->cpu->trap(fault, this->threadNumber, this->staticInst);
}

template <class Impl>
void
BaseO3DynInst<Impl>::syscall()
{
    // HACK: check CPU's nextPC before and after syscall. If it
    // changes, update this instruction's nextPC because the syscall
    // must have changed the nextPC.
    TheISA::PCState curPC = this->cpu->pcState(this->threadNumber);
    this->cpu->syscall(this->threadNumber);
    TheISA::PCState newPC = this->cpu->pcState(this->threadNumber);
    if (!(curPC == newPC)) {
        this->pcState(newPC);
    }
}

#endif//__CPU_O3_DYN_INST_IMPL_HH__
