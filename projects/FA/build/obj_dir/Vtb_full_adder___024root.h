// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_full_adder.h for the primary calling header

#ifndef VERILATED_VTB_FULL_ADDER___024ROOT_H_
#define VERILATED_VTB_FULL_ADDER___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_full_adder__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_full_adder___024root final {
  public:

    // TRIVIALLY CONSTRUCTIBLE STATE
    ::CData/*0:0*/ tb_full_adder__DOT__a;
    ::CData/*0:0*/ tb_full_adder__DOT__b;
    ::CData/*0:0*/ tb_full_adder__DOT__cin;
    ::CData/*0:0*/ tb_full_adder__DOT__dut__DOT__sum;
    ::CData/*0:0*/ tb_full_adder__DOT__dut__DOT__cout;
    ::IData/*31:0*/ __Vi;
    ::VlUnpacked<::QData/*63:0*/, 1> __VstlTriggered;
    ::VlUnpacked<::QData/*63:0*/, 1> __VicoTriggered;
    ::VlUnpacked<::QData/*63:0*/, 1> __VactTriggered;
    ::VlUnpacked<::QData/*63:0*/, 1> __VactTriggeredAcc;
    ::VlUnpacked<::QData/*63:0*/, 1> __VnbaTriggered;
    ::VlUnpacked<::CData/*0:0*/, 3> __VtraceActivity;

    // NOT TRIVIALLY CONSTRUCTIBLE STATE
    union {
        struct {
            ::VlDelayScheduler __VdlySched;
        } __Vnontrivs;
    };

    // INTERNAL VARIABLES
    Vtb_full_adder__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_full_adder___024root(Vtb_full_adder__Syms* symsp, const char* namep);
    ~Vtb_full_adder___024root();
    VL_UNCOPYABLE(Vtb_full_adder___024root);
    VL_UNMOVABLE(Vtb_full_adder___024root);

    // INTERNAL METHODS
};


#endif  // guard
