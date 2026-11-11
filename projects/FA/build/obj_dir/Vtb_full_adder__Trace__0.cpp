// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals

#include "verilated_vcd_c.h"
#include "Vtb_full_adder__Syms.h"


void Vtb_full_adder___024root__trace_chg_0_sub_0(Vtb_full_adder___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vtb_full_adder___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root__trace_chg_0\n"); );
    // Body
    Vtb_full_adder___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_full_adder___024root*>(voidSelf);
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    Vtb_full_adder___024root__trace_chg_0_sub_0((&vlSymsp->vlInst.TOP), bufp);
}

void Vtb_full_adder___024root__trace_chg_0_sub_0(Vtb_full_adder___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root__trace_chg_0_sub_0\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 0);
    if (VL_UNLIKELY(((vlSelfRef.__VtraceActivity[1U] 
                      | vlSelfRef.__VtraceActivity[2U])))) {
        bufp->chgBit(oldp+0,(vlSelfRef.tb_full_adder__DOT__a));
        bufp->chgBit(oldp+1,(vlSelfRef.tb_full_adder__DOT__b));
        bufp->chgBit(oldp+2,(vlSelfRef.tb_full_adder__DOT__cin));
        bufp->chgBit(oldp+3,(((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                              ^ ((::IData)(vlSelfRef.tb_full_adder__DOT__b) 
                                 ^ (::IData)(vlSelfRef.tb_full_adder__DOT__cin)))));
        bufp->chgBit(oldp+4,((((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                               & (::IData)(vlSelfRef.tb_full_adder__DOT__b)) 
                              | ((::IData)(vlSelfRef.tb_full_adder__DOT__cin) 
                                 & ((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                                    | (::IData)(vlSelfRef.tb_full_adder__DOT__b))))));
    }
}

void Vtb_full_adder___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root__trace_cleanup\n"); );
    // Body
    Vtb_full_adder___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_full_adder___024root*>(voidSelf);
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    vlSymsp->__Vm_activity = false;
    vlSymsp->vlInst.TOP.__VtraceActivity[0U] = 0U;
    vlSymsp->vlInst.TOP.__VtraceActivity[1U] = 0U;
    vlSymsp->vlInst.TOP.__VtraceActivity[2U] = 0U;
}
