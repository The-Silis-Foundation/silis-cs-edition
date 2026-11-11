// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_full_adder.h for the primary calling header

#include "Vtb_full_adder__pch.h"

VlCoroutine Vtb_full_adder___024root___eval_initial__TOP__Vtiming__0(Vtb_full_adder___024root* vlSelf);

void Vtb_full_adder___024root___eval_initial(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_initial\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_full_adder___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    vlSelfRef.__VtraceActivity[1U] = 1U;
}

void Vtb_full_adder___024root___eval_sample(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_sample\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_full_adder___024root___dump_triggers__ico(const ::VlUnpacked<::QData/*63:0*/, 1> &triggers, const ::std::string &tag);
#endif  // VL_DEBUG

bool Vtb_full_adder___024root___eval_ico(Vtb_full_adder___024root* vlSelf, ::CData/*0:0*/ firstIteration) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_ico\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[0U]) 
                                     | (::IData)((::IData)(firstIteration)));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_full_adder___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
    return (0U);
}

void Vtb_full_adder___024root___trigger_orInto__act_vec_vec(::VlUnpacked<::QData/*63:0*/, 1> &out, const ::VlUnpacked<::QData/*63:0*/, 1> &in);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_full_adder___024root___dump_triggers__act(const ::VlUnpacked<::QData/*63:0*/, 1> &triggers, const ::std::string &tag);
#endif  // VL_DEBUG
bool Vtb_full_adder___024root___trigger_anySet__act(const ::VlUnpacked<::QData/*63:0*/, 1> &in);

bool Vtb_full_adder___024root___eval_act(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_act\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    ::CData/*0:0*/ __VactExecute;
    // Body
    {
        // Inlined CFunc: _eval_triggers_vec__act
        vlSelfRef.__VactTriggered[0U] = (::QData)((::IData)(vlSelfRef.__Vnontrivs.__VdlySched.awaitingCurrentTime()));
    }
    Vtb_full_adder___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VactTriggered, vlSelfRef.__VactTriggeredAcc);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_full_adder___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtb_full_adder___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_full_adder___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        vlSelfRef.__VactTriggeredAcc.fill(0ULL);
        {
            // Inlined CFunc: _timing_resume
            if ((((1ULL & vlSelfRef.__VactTriggered[0U])) != 0ULL)) {
                vlSelfRef.__Vnontrivs.__VdlySched.resume();
            }
        }
        {
            // Inlined CFunc: _eval_body__act
            if ((((1ULL & vlSelfRef.__VactTriggered[0U])) != 0ULL)) {
                {
                    // Inlined CFunc: _act_comb__TOP__0
                    vlSelfRef.tb_full_adder__DOT__dut__DOT__cout 
                        = (((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                            & (::IData)(vlSelfRef.tb_full_adder__DOT__b)) 
                           | ((::IData)(vlSelfRef.tb_full_adder__DOT__cin) 
                              & ((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                                 | (::IData)(vlSelfRef.tb_full_adder__DOT__b))));
                    vlSelfRef.tb_full_adder__DOT__dut__DOT__sum 
                        = ((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                           ^ ((::IData)(vlSelfRef.tb_full_adder__DOT__b) 
                              ^ (::IData)(vlSelfRef.tb_full_adder__DOT__cin)));
                }
            }
        }
    }
    return (__VactExecute);
}

bool Vtb_full_adder___024root___eval_inact(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_inact\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    ::CData/*0:0*/ __VinactExecute;
    // Body
    __VinactExecute = vlSelfRef.__Vnontrivs.__VdlySched.awaitingZeroDelay();
    if (__VinactExecute) {
        VL_FATAL_MT("/home/econsystems/silisfoundation/silis-cs-edition/projects/FA/tb/tb_FA.v", 2, "", "ZERODLY: Design Verilated with '--no-sched-zero-delay', but #0 delay executed at runtime");
    }
    return (__VinactExecute);
}

void Vtb_full_adder___024root___trigger_clear__act(::VlUnpacked<::QData/*63:0*/, 1> &out);

bool Vtb_full_adder___024root___eval_nba(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_nba\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    ::CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_full_adder___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        {
            // Inlined CFunc: _eval_body__nba
            if ((((1ULL & vlSelfRef.__VnbaTriggered[0U])) != 0ULL)) {
                {
                    // Inlined CFunc: _act_comb__TOP__0
                    vlSelfRef.tb_full_adder__DOT__dut__DOT__cout 
                        = (((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                            & (::IData)(vlSelfRef.tb_full_adder__DOT__b)) 
                           | ((::IData)(vlSelfRef.tb_full_adder__DOT__cin) 
                              & ((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                                 | (::IData)(vlSelfRef.tb_full_adder__DOT__b))));
                    vlSelfRef.tb_full_adder__DOT__dut__DOT__sum 
                        = ((::IData)(vlSelfRef.tb_full_adder__DOT__a) 
                           ^ ((::IData)(vlSelfRef.tb_full_adder__DOT__b) 
                              ^ (::IData)(vlSelfRef.tb_full_adder__DOT__cin)));
                }
            }
        }
        Vtb_full_adder___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

bool Vtb_full_adder___024root___eval_obs(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_obs\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

bool Vtb_full_adder___024root___eval_react(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_react\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

void Vtb_full_adder___024root___eval_postponed(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_postponed\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VlCoroutine Vtb_full_adder___024root___eval_initial__TOP__Vtiming__0(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    ::IData/*31:0*/ tb_full_adder__DOT__unnamedblk1__DOT__i;
    tb_full_adder__DOT__unnamedblk1__DOT__i = 0;
    // Body
    vlSymsp->_vm_contextp__->dumpfile("waveform.vcd"s);
    vlSymsp->_traceDumpOpen();
    VL_WRITEF_NX("Time\t A B Cin | Cout Sum\n-----------------------\n",0);
    tb_full_adder__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 8U, tb_full_adder__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.tb_full_adder__DOT__a = (1U & (tb_full_adder__DOT__unnamedblk1__DOT__i 
                                                 >> 2U));
        vlSelfRef.tb_full_adder__DOT__b = (1U & (tb_full_adder__DOT__unnamedblk1__DOT__i 
                                                 >> 1U));
        vlSelfRef.tb_full_adder__DOT__cin = (1U & tb_full_adder__DOT__unnamedblk1__DOT__i);
        co_await vlSelfRef.__Vnontrivs.__VdlySched.delay(0x0000000000002710ULL, 
                                                         nullptr, 
                                                         "/home/econsystems/silisfoundation/silis-cs-edition/projects/FA/tb/tb_FA.v", 
                                                         31);
        vlSelfRef.__VtraceActivity[2U] = 1U;
        VL_WRITEF_NX("%0t\t %b %b  %b  |  %b    %b\n",7, 'T',-9
                     , '#',64,VL_TIME_UNITED_Q(1000)
                     , '#',1,(::IData)(vlSelfRef.tb_full_adder__DOT__a)
                     , '#',1,vlSelfRef.tb_full_adder__DOT__b
                     , '#',1,(::IData)(vlSelfRef.tb_full_adder__DOT__cin)
                     , '#',1,vlSelfRef.tb_full_adder__DOT__dut__DOT__cout
                     , '#',1,(::IData)(vlSelfRef.tb_full_adder__DOT__dut__DOT__sum));
        tb_full_adder__DOT__unnamedblk1__DOT__i = ((::IData)(1U) 
                                                   + tb_full_adder__DOT__unnamedblk1__DOT__i);
    }
    VL_FINISH_MT("/home/econsystems/silisfoundation/silis-cs-edition/projects/FA/tb/tb_FA.v", 35, "");
    vlSelfRef.__VtraceActivity[2U] = 1U;
    co_return;
}

bool Vtb_full_adder___024root___trigger_anySet__ico(const ::VlUnpacked<::QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___trigger_anySet__ico\n"); );
    // Locals
    ::IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((::IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

bool Vtb_full_adder___024root___trigger_anySet__act(const ::VlUnpacked<::QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___trigger_anySet__act\n"); );
    // Locals
    ::IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((::IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtb_full_adder___024root___trigger_orInto__act_vec_vec(::VlUnpacked<::QData/*63:0*/, 1> &out, const ::VlUnpacked<::QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    ::IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((::IData)(1U) + n);
    } while ((0U >= n));
}

void Vtb_full_adder___024root___trigger_clear__act(::VlUnpacked<::QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___trigger_clear__act\n"); );
    // Locals
    ::IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((::IData)(1U) + n);
    } while ((1U > n));
}

#ifdef VL_DEBUG
void Vtb_full_adder___024root___eval_debug_assertions(Vtb_full_adder___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_full_adder___024root___eval_debug_assertions\n"); );
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
