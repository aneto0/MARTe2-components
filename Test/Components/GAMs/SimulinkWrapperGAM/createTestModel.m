function model_compiled = createTestModel(hasAllocFcn, hasGetmmiFcn)

warning('off', 'all');

global model_name  model_compiled

model_name = ['test_model' int2str(hasAllocFcn) int2str(hasGetmmiFcn)];

model_compiled = false;

if isfile([model_name '.so'])
    model_compiled = true;
    return
end

%% define constants

evalin('base', 'matrixConstant = [1 1 1; 2 2 2; 3 3 3];');
evalin('base', 'vectorConstant = ones(10,1);');

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
add_block('simulink/Sources/Constant',        [model_name '/Constant1']);
add_block('simulink/Sources/Constant',        [model_name '/Constant2']);
add_block('simulink/Sources/Constant',        [model_name '/Constant3']);
add_block('simulink/Sources/Pulse Generator', [model_name '/Pulse1']);
add_block('simulink/Signal Attributes/Data Type Conversion', [model_name '/Convert1']);
add_block('simulink/Signal Attributes/Data Type Conversion', [model_name '/Convert2']);
add_block('simulink/Signal Routing/Mux',      [model_name '/Mux1']);
add_block('simulink/Math Operations/Matrix Concatenate', [model_name '/Concatenate1']);
add_block('simulink/Math Operations/Add',     [model_name '/Add1']);
add_block('simulink/Math Operations/Gain',    [model_name '/Gain1']);

% subsystems
add_block('simulink/Ports & Subsystems/Subsystem',[model_name '/Subsystem1']);

% in/out ports
add_block('simulink/Sources/In1', [model_name '/In1_ScalarDouble']);
add_block('simulink/Sources/In1', [model_name '/In2_ScalarSingle' ]);
add_block('simulink/Sources/In1', [model_name '/In3_ScalarInt8'  ]);
add_block('simulink/Sources/In1', [model_name '/In4_VectorDouble']);
add_block('simulink/Sources/In1', [model_name '/In5_VectorSingle']);

add_block('simulink/Sinks/Out1',  [model_name '/Out1_ScalarDouble']);
add_block('simulink/Sinks/Out1',  [model_name '/Out2_ScalarSingle']);
add_block('simulink/Sinks/Out1',  [model_name '/Out4_VectorDouble']);
add_block('simulink/Sinks/Out1',  [model_name '/Out8_MatrixSingle']);
add_block('simulink/Sinks/Out1',  [model_name '/Out9_MatrixDouble']);

% -- inside Subsystem1

% normal blocks
add_block('simulink/Math Operations/Add', [model_name '/Subsystem1/Add1']);

% subsystems

% in/out ports
add_block('simulink/Sources/In1', [model_name '/Subsystem1/In2']);
add_block('simulink/Sources/In1', [model_name '/Subsystem1/In3']);

%% set block properties

% normal blocks
set_param([model_name '/Convert1'], 'OutDataTypeStr', 'double');
set_param([model_name '/Convert2'], 'OutDataTypeStr', 'single');

set_param([model_name '/Constant1'], 'Value', '10');

set_param([model_name '/Constant2'], 'Value',          'matrixConstant');
set_param([model_name '/Constant2'], 'OutDataTypeStr', 'double');

set_param([model_name '/Constant3'], 'Value',          'vectorConstant');
set_param([model_name '/Constant3'], 'OutDataTypeStr', 'single');

set_param([model_name '/Gain1'],     'Gain',          '1');

set_param([model_name '/Add1'],      'Inputs',         '++');

% in/out ports
set_param([model_name '/In1_ScalarDouble'],  'IconDisplay',    'Signal name');
set_param([model_name '/In1_ScalarDouble'],  'OutDataTypeStr', 'double');

set_param([model_name '/In2_ScalarSingle'],  'IconDisplay',    'Signal name');
set_param([model_name '/In2_ScalarSingle'],  'OutDataTypeStr', 'single');

set_param([model_name '/In3_ScalarInt8'],    'IconDisplay',    'Signal name');
set_param([model_name '/In3_ScalarInt8'],    'OutDataTypeStr', 'int8');

set_param([model_name '/In4_VectorDouble'],  'IconDisplay',    'Signal name');
set_param([model_name '/In4_VectorDouble'],  'OutDataTypeStr', 'double');
set_param([model_name '/In4_VectorDouble'],  'PortDimensions', '9');

set_param([model_name '/In5_VectorSingle'],  'IconDisplay',    'Signal name');
set_param([model_name '/In5_VectorSingle'],  'OutDataTypeStr', 'single');
set_param([model_name '/In5_VectorSingle'],  'PortDimensions', '10');

set_param([model_name '/Out1_ScalarDouble'], 'IconDisplay',    'Signal name');

set_param([model_name '/Out2_ScalarSingle'],  'IconDisplay',    'Signal name');
set_param([model_name '/Out2_ScalarSingle'],  'OutDataTypeStr', 'single');

set_param([model_name '/Out4_VectorDouble'], 'IconDisplay',    'Signal name');
set_param([model_name '/Out4_VectorDouble'], 'OutDataTypeStr', 'double');
set_param([model_name '/Out4_VectorDouble'], 'PortDimensions', '10');

set_param([model_name '/Out8_MatrixSingle'], 'IconDisplay',    'Signal name');
set_param([model_name '/Out8_MatrixSingle'], 'OutDataTypeStr', 'single');
set_param([model_name '/Out8_MatrixSingle'], 'PortDimensions', '[10 2]');

set_param([model_name '/Out9_MatrixDouble'], 'IconDisplay',    'Signal name');
set_param([model_name '/Out9_MatrixDouble'], 'OutDataTypeStr', 'double');
set_param([model_name '/Out9_MatrixDouble'], 'PortDimensions', '[3 3]');

set_param([model_name '/Subsystem1/Add1'], 'Inputs', '+++');

%% manage connections

% delete default connection between In1 and Out1 in Subsystem1
delete_line([model_name '/Subsystem1'], 'In1/1', 'Out1/1');

% three methods of managing connections:
% 1. by using exact port coordinates
% 2. by using port handles
% 3. by using port names and IDs

% method 1: get exact coordinates of the block port
% Constant1_Port      = get_param([model_name '/Constant1']      , 'PortConnectivity');
% Subsystem1_Port = get_param([model_name '/Subsystem1'] , 'PortConnectivity');
% 
% add_line(model_name, [Constant1_Port.Position; Subsystem1_Port(1).Position]);

% method 1, but using three segments of line
% Pulse1_Port      = get_param([model_name '/Pulse1']      , 'PortConnectivity');
% 
% add_line(model_name, [Pulse1_Port.Position; 275 220]);
% add_line(model_name, [275 220; 275 180]);
% add_line(model_name, [275 180; Subsystem1_Port(2).Position]);

% method 2
% SubsysPortHandles = get_param([model_name '/Subsystem1'] ,'PortHandles');
% ScopePortHandles  = get_param([model_name '/Scope1']     ,'PortHandles');
% add_line(model_name, SubsysPortHandles.Outport(1), ScopePortHandles.Inport(1));

% method 3
add_line(model_name, 'Constant1/1', 'Subsystem1/1');
add_line(model_name, 'Pulse1/1', 'Subsystem1/2');

add_line([model_name '/Subsystem1'],'In1/1','Add1/1');
add_line([model_name '/Subsystem1'],'In2/1','Add1/2');
add_line([model_name '/Subsystem1'],'Add1/1','Out1/1');

add_line(model_name, 'In1_ScalarDouble/1', 'Subsystem1/3');
add_line(model_name, 'Subsystem1/1',       'Out1_ScalarDouble/1');
add_line(model_name, 'In3_ScalarInt8/1',   'Convert1/1');
add_line(model_name, 'Convert1/1',         'Mux1/1');
add_line(model_name, 'In4_VectorDouble/1', 'Mux1/2');
add_line(model_name, 'Mux1/1',             'Out4_VectorDouble/1');
add_line(model_name, 'Mux1/1',             'Convert2/1');
add_line(model_name, 'Convert2/1',         'Add1/1');
add_line(model_name, 'Add1/1',             'Concatenate1/1');
add_line(model_name, 'Constant3/1',        'Add1/2');
add_line(model_name, 'In5_VectorSingle/1', 'Concatenate1/2');
add_line(model_name, 'Concatenate1/1',     'Out8_MatrixSingle/1');
add_line(model_name, 'Constant2/1',        'Out9_MatrixDouble/1');
add_line(model_name, 'In2_ScalarSingle/1', 'Gain1/1');
add_line(model_name, 'Gain1/1',            'Out2_ScalarSingle/1');

add_line([model_name '/Subsystem1'], 'In3/1', 'Add1/3');


%% signal managing

% name the signal exiting from Constant1
p = get_param([model_name '/Pulse1'], 'PortHandles');
l = get_param(p.Outport,'Line');
set_param(l,'Name','PulseSignal');

p = get_param([model_name '/In1_ScalarDouble'], 'PortHandles');
l = get_param(p.Outport,'Line');
set_param(l,'Name','In1_ScalarDouble');

p = get_param([model_name '/Out1_ScalarDouble'], 'PortHandles');
l = get_param(p.Inport,'Line');
set_param(l,'Name','Out1_ScalarDouble');

p = get_param([model_name '/In3_ScalarInt8'], 'PortHandles');
l = get_param(p.Outport,'Line');
set_param(l,'Name','In3_ScalarInt8');

name_input_signal([model_name '/Out2_ScalarSingle'], 1, 'Out2_ScalarSingle');
name_input_signal([model_name '/Out8_MatrixSingle'], 1, 'Out8_MatrixSingle');
name_input_signal([model_name '/Out4_VectorDouble'], 1, 'Out4_VectorDouble');
name_input_signal([model_name '/Out9_MatrixDouble'], 1, 'Out9_MatrixDouble');

name_output_signal([model_name '/In2_ScalarSingle'], 1, 'In2_ScalarSingle');
name_output_signal([model_name '/In4_VectorDouble'], 1, 'In4_VectorDouble');
name_output_signal([model_name '/In5_VectorSingle'], 1, 'In5_VectorSingle');

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
if hasGetmmiFcn == true
    set_param(model_name, 'CustomSourceCode', ...
        [ 'void* ' model_name '_GetCAPImmi(void* voidPtrToRealTimeStructure)' newline, ...
        '{' newline, ...
        '    rtwCAPI_ModelMappingInfo* mmiPtr = &(rtmGetDataMapInfo((RT_MODEL_' model_name '_T*)(voidPtrToRealTimeStructure)).mmi);' newline, ...
        '   return (void*) mmiPtr;' newline, ...
        '}' ] ...
    );
end

% Interface
set_param(model_name, 'SupportComplex', 0);
set_param(model_name, 'SupportAbsoluteTime', 0);
set_param(model_name, 'SuppressErrorStatus', 1);

set_param(model_name, 'CodeInterfacePackaging', 'Reusable function');

set_param(model_name, 'RootIOFormat', 'Part of model data structure');

set_param(model_name, 'RTWCAPIParams', 1);
set_param(model_name, 'RTWCAPIRootIO', 1);

if hasAllocFcn == true
    set_param(model_name, 'GenerateAllocFcn', 1);
end

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
delete(sprintf('%s.slx.bak',model_name));

warning('on', 'all');

end   % function

function name_input_signal(address, signal_index, signal_name)
    
    p = get_param(address, 'PortHandles');
    l = get_param(p.Inport(signal_index),'Line');
    set_param(l,'Name', signal_name);
    
end

function name_output_signal(address, signal_index, signal_name)
    
    p = get_param(address, 'PortHandles');
    l = get_param(p.Outport(signal_index),'Line');
    set_param(l,'Name', signal_name);
    
end