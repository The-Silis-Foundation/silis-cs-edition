# GUI as a Projection

The Silis graphical interface is a **pure visual lens** over the Python SDK.

## The Mental Model
The GUI does not "own" the layout.
The GUI does not compute physical coordinates, generate power grids, or make placement decisions.
All layout math, DRC verification, and design state mutations live strictly within `silis.core.sdk`.

## Dataflow Contract
1. User clicks or drags on the canvas.
2. The Qt event calls an SDK method (`MacroInstance.move_to()`, `Floorplan.add_macro()`).
3. The SDK validates physical rules and legalizes the state.
4. The SDK updates the database and emits state change signals.
5. The Qt canvas redraws the new state.

Any operation achievable in the GUI must be 100% reproducible in a headless 5-line Python script using the SDK.

## Related Notes
- [[00_Index]]
- [[01_Agent_Directives_EDA_Parity]]
- [[decisions/Authoritative_SDK_State]]
