// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_full_adder__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vtb_full_adder::Vtb_full_adder(VerilatedContext* _vcontextp__, const char* _vcname__)
    : Vtb_full_adder{_vcontextp__, _vcname__, true} {}

Vtb_full_adder::Vtb_full_adder(VerilatedContext* _vcontextp__, const char* _vcname__, bool _vcisRoot__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_full_adder__Syms(contextp(), this, _vcname__, _vcisRoot__)}
    , m_evalLoop{*this, /*convergeLimit:*/ 10000}
    , rootp{&(vlSymsp->vlInst.TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
    contextp()->traceBaseModelCbAdd(
        [this](VerilatedTraceBaseC* tfp, int levels, int options) { traceBaseModel(tfp, levels, options); });
}

Vtb_full_adder::Vtb_full_adder(const char* _vcname__)
    : Vtb_full_adder(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_full_adder::~Vtb_full_adder() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_full_adder___024root___eval_debug_assertions(Vtb_full_adder___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD void Vtb_full_adder___024root___eval_static(Vtb_full_adder___024root* vlSelf);
void Vtb_full_adder___024root___eval_initial(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD bool Vtb_full_adder___024root___eval_stl(Vtb_full_adder___024root* vlSelf, ::CData/*0:0*/ firstIteration);
void Vtb_full_adder___024root___eval_sample(Vtb_full_adder___024root* vlSelf);
bool Vtb_full_adder___024root___eval_ico(Vtb_full_adder___024root* vlSelf, ::CData/*0:0*/ firstIteration);
bool Vtb_full_adder___024root___eval_act(Vtb_full_adder___024root* vlSelf);
bool Vtb_full_adder___024root___eval_inact(Vtb_full_adder___024root* vlSelf);
bool Vtb_full_adder___024root___eval_nba(Vtb_full_adder___024root* vlSelf);
bool Vtb_full_adder___024root___eval_obs(Vtb_full_adder___024root* vlSelf);
bool Vtb_full_adder___024root___eval_react(Vtb_full_adder___024root* vlSelf);
void Vtb_full_adder___024root___eval_postponed(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_final(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__stl(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__ico(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__act(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__nba(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__obs(Vtb_full_adder___024root* vlSelf);
VL_ATTR_COLD void Vtb_full_adder___024root___eval_dump_triggers__react(Vtb_full_adder___024root* vlSelf);

void Vtb_full_adder::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_full_adder::eval_step\n"); );
    m_evalLoop.eval();
}

void Vtb_full_adder::evalBegin() {
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_full_adder___024root___eval_debug_assertions(&(vlSymsp->vlInst.TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
}

void Vtb_full_adder::evalEnd() {
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
    vlSymsp->vlInst.TOP.__Vnontrivs.__VdlySched.cleanupForevered();
}

void Vtb_full_adder::evalStatic() {
    Vtb_full_adder___024root___eval_static(&(vlSymsp->vlInst.TOP));
}

void Vtb_full_adder::evalInitial() {
    Vtb_full_adder___024root___eval_initial(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalStl(bool firstIteration) {
    return Vtb_full_adder___024root___eval_stl(&(vlSymsp->vlInst.TOP), firstIteration);
}

void Vtb_full_adder::evalSample() {
    Vtb_full_adder___024root___eval_sample(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalIco(bool firstIteration) {
    return Vtb_full_adder___024root___eval_ico(&(vlSymsp->vlInst.TOP), firstIteration);
}

bool Vtb_full_adder::evalAct() {
    return Vtb_full_adder___024root___eval_act(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalInact() {
    return Vtb_full_adder___024root___eval_inact(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalNba() {
    return Vtb_full_adder___024root___eval_nba(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalObs() {
    return Vtb_full_adder___024root___eval_obs(&(vlSymsp->vlInst.TOP));
}

bool Vtb_full_adder::evalReact() {
    return Vtb_full_adder___024root___eval_react(&(vlSymsp->vlInst.TOP));
}

void Vtb_full_adder::evalPostponed() {
    Vtb_full_adder___024root___eval_postponed(&(vlSymsp->vlInst.TOP));
}

void Vtb_full_adder::evalFinal() {
    Vtb_full_adder___024root___eval_final(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersStl() {
    Vtb_full_adder___024root___eval_dump_triggers__stl(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersIco() {
    Vtb_full_adder___024root___eval_dump_triggers__ico(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersAct() {
    Vtb_full_adder___024root___eval_dump_triggers__act(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersNba() {
    Vtb_full_adder___024root___eval_dump_triggers__nba(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersObs() {
    Vtb_full_adder___024root___eval_dump_triggers__obs(&(vlSymsp->vlInst.TOP));
}

VL_ATTR_COLD void Vtb_full_adder::dumpTriggersReact() {
    Vtb_full_adder___024root___eval_dump_triggers__react(&(vlSymsp->vlInst.TOP));
}

void Vtb_full_adder::eval_end_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+eval_end_step Vtb_full_adder::eval_end_step\n"); );
#ifdef VM_TRACE
    // Tracing
    if (VL_UNLIKELY(vlSymsp->__Vm_dumping)) vlSymsp->_traceDump();
#endif  // VM_TRACE
}

//============================================================
// Events and timing
bool Vtb_full_adder::eventsPending() { return !vlSymsp->vlInst.TOP.__Vnontrivs.__VdlySched.empty() && !contextp()->gotFinish(); }

uint64_t Vtb_full_adder::nextTimeSlot() { return vlSymsp->vlInst.TOP.__Vnontrivs.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_full_adder::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

VL_ATTR_COLD void Vtb_full_adder::final() {
    contextp()->executingFinal(true);
    evalFinal();
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_full_adder::hierName() const { return vlSymsp->name(); }
const char* Vtb_full_adder::modelName() const { return "Vtb_full_adder"; }
unsigned Vtb_full_adder::threads() const { return 1; }
void Vtb_full_adder::prepareClone() const { contextp()->prepareClone(); }
void Vtb_full_adder::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vtb_full_adder::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false}};
};

//============================================================
// Trace configuration

void Vtb_full_adder___024root__trace_decl_types(VerilatedVcd* tracep);

void Vtb_full_adder___024root__trace_init_top(Vtb_full_adder___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vtb_full_adder___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_full_adder___024root*>(voidSelf);
    Vtb_full_adder__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(vlSymsp->name(), VerilatedTracePrefixType::SCOPE_MODULE);
    Vtb_full_adder___024root__trace_decl_types(tracep);
    Vtb_full_adder___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtb_full_adder___024root__trace_register(Vtb_full_adder___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vtb_full_adder::traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options) {
    (void)levels; (void)options;
    VerilatedVcdC* const stfp = dynamic_cast<VerilatedVcdC*>(tfp);
    if (VL_UNLIKELY(!stfp)) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtb_full_adder::trace()' called on non-VerilatedVcdC object;"
            " use --trace-fst with VerilatedFst object, and --trace-vcd with VerilatedVcd object");
    }
    stfp->spTrace()->addModel(this);
    stfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->vlInst.TOP), name(), false, 5);
    Vtb_full_adder___024root__trace_register(&(vlSymsp->vlInst.TOP), stfp->spTrace());
}
