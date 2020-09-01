close all; clear variables; clc;

model_name = 'test_model';

model_compiled = false;

%% creating a new model

% delete model if it already exists
warning('off','MATLAB:DELETE:FileNotFound');
delete(sprintf('%s.slx',model_name));
warning('on','MATLAB:DELETE:FileNotFound');

% create the new model
new_system(model_name);
save_system(model_name);

%% adding blocks

% -- root system

% normal blocks
add_block('simulink/Sources/Constant',        [model_name '/Sine1']);
add_block('simulink/Sources/Pulse Generator', [model_name '/Pulse1']);
add_block('simulink/Sinks/Scope',             [model_name '/Scope1']);

% subsystems
add_block('simulink/Ports & Subsystems/Subsystem',[model_name '/Subsystem1']);

% in/out ports
add_block('simulink/Sources/In1', [model_name '/RootIn1']);
add_block('simulink/Sinks/Out1', [model_name '/RootOut1']);

% -- inside Subsystem1

% normal blocks
add_block('simulink/Math Operations/Add', [model_name '/Subsystem1/Add1']);

% subsystems

% in/out ports
add_block('simulink/Sources/In1', [model_name '/Subsystem1/In2']);
add_block('simulink/Sources/In1', [model_name '/Subsystem1/In3']);

%% set block properties

set_param([model_name '/Sine1'], 'Value', '10');

set_param([model_name '/RootIn1'], 'IconDisplay', 'Signal name');
set_param([model_name '/RootOut1'], 'IconDisplay', 'Signal name');

set_param([model_name '/Subsystem1/Add1'], 'Inputs', '+++');

%% manage connections

% delete default connection between In1 and Out1 in Subsystem1
delete_line([model_name '/Subsystem1'], 'In1/1', 'Out1/1');

% three methods of managing connections:
% 1. by using exact port coordinates
% 2. by using port handles
% 3. by using port names and IDs

% method 1: get exact coordinates of the block port
Sine1_Port      = get_param([model_name '/Sine1']      , 'PortConnectivity');
Subsystem1_Port = get_param([model_name '/Subsystem1'] , 'PortConnectivity');

add_line(model_name, [Sine1_Port.Position; Subsystem1_Port(1).Position]);

% method 1, but using three segments of line
Pulse1_Port      = get_param([model_name '/Pulse1']      , 'PortConnectivity');

add_line(model_name, [Pulse1_Port.Position; 275 220]);
add_line(model_name, [275 220; 275 180]);
add_line(model_name, [275 180; Subsystem1_Port(2).Position]);

% method 2
SubsysPortHandles = get_param([model_name '/Subsystem1'] ,'PortHandles');
ScopePortHandles  = get_param([model_name '/Scope1']     ,'PortHandles');
add_line(model_name, SubsysPortHandles.Outport(1), ScopePortHandles.Inport(1));

% method 3
add_line([model_name '/Subsystem1'],'In1/1','Add1/1');
add_line([model_name '/Subsystem1'],'In2/1','Add1/2');
add_line([model_name '/Subsystem1'],'Add1/1','Out1/1');

add_line(model_name, 'RootIn1/1', 'Subsystem1/3');
add_line(model_name, 'Subsystem1/1', 'RootOut1/1');

add_line([model_name '/Subsystem1'], 'In3/1', 'Add1/3');


%% signal managing

% name the signal exiting from Sine1
p = get_param([model_name '/Pulse1'], 'PortHandles');
l = get_param(p.Outport,'Line');
set_param(l,'Name','FooSignal');

p = get_param([model_name '/RootIn1'], 'PortHandles');
l = get_param(p.Outport,'Line');
set_param(l,'Name','RootIn1');

p = get_param([model_name '/RootOut1'], 'PortHandles');
l = get_param(p.Inport,'Line');
set_param(l,'Name','RootOut1');

%% arranging block layout
% alternatively to setting the position of each block, the system can be
% arranged automatically
Simulink.BlockDiagram.arrangeSystem(model_name);
Simulink.BlockDiagram.arrangeSystem([model_name '/Subsystem1']);

%% code generation
% name of each option is available by right-clicking on the option name
% in Model Settings dialog and then on "What's This?"

% Solver
set_param(model_name, 'SolverType', 'Fixed-step');

% Code Generation
set_param(model_name, 'SystemTargetFile', 'ert_shrlib.tlc');

% Optimization
set_param(model_name, 'DefaultParameterBehavior', 'Tunable');

set_param(model_name, 'OptimizationCustomize', 1);
set_param(model_name, 'GlobalVariableUsage', 'None');

% Report
set_param(model_name, 'GenerateReport', 0);

% Comments
set_param(model_name, 'GenerateComments', 0);

% Custom code
set_param(model_name, 'CustomSourceCode', ...
    [ 'void* ' model_name '_GetCAPImmi(void* voidPtrToRealTimeStructure)' newline, ...
    '{' newline, ...
    '    rtwCAPI_ModelMappingInfo* mmiPtr = &(rtmGetDataMapInfo((RT_MODEL_' model_name '_T*)(voidPtrToRealTimeStructure)).mmi);' newline, ...
    '   return (void*) mmiPtr;' newline, ...
    '}' ] ...
);

% Interface
set_param(model_name, 'SupportComplex', 0);
set_param(model_name, 'SupportAbsoluteTime', 0);
set_param(model_name, 'SuppressErrorStatus', 1);

set_param(model_name, 'CodeInterfacePackaging', 'Reusable function');

set_param(model_name, 'RootIOFormat', 'Part of model data structure');

set_param(model_name, 'RTWCAPIParams', 1);
set_param(model_name, 'RTWCAPIRootIO', 1);

set_param(model_name, 'GenerateAllocFcn', 1);

set_param(model_name, 'IncludeMdlTerminateFcn', 0);
set_param(model_name, 'CombineSignalStateStructs', 1);

try
    rtwbuild(model_name)
    model_compiled = true;
catch
    model_compiled = false;
end
    
    
%% save and close
save_system(model_name);
close_system(model_name);

% clean build directory
rmdir('slprj', 's');
rmdir([model_name '_ert_shrlib_rtw'], 's');
delete(sprintf('%s.slx',model_name));
delete(sprintf('%s.slxc',model_name));

















