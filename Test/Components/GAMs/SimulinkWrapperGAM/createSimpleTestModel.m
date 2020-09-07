function model_compiled = createSimpleTestModel(hasAllocFcn, hasGetmmiFcn, hasStructArrayParams)

global model_name  model_compiled

model_name = ['simple_test_model' int2str(hasAllocFcn) int2str(hasGetmmiFcn) int2str(hasStructArrayParams)];

model_compiled = false;

%% define constants

evalin('base', 'matrixConstant = [1 1 1; 2 2 2; 3 3 3];');
evalin('base', 'vectorConstant = ones(10,1);');

if hasStructArrayParams == true
    
    evalin('base', 'structArray(1).one = 1;');
    evalin('base', 'structArray(1).two = 2;');
    evalin('base', 'structArray(2).one = 10;');
    evalin('base', 'structArray(2).two = 20;');
    
end

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

% math blocks
add_block('simulink/Math Operations/Gain',    [model_name '/Gain1']);
add_block('simulink/Math Operations/Gain',    [model_name '/Gain2']);

% in/out ports
add_block('simulink/Sources/In1', [model_name '/In1_ScalarDouble']);
add_block('simulink/Sources/In1', [model_name '/In2_ScalarUint32' ]);

add_block('simulink/Sinks/Out1',  [model_name '/Out1_ScalarDouble']);
add_block('simulink/Sinks/Out1',  [model_name '/Out2_ScalarUint32']);

%% set block properties

% math blocks
if hasStructArrayParams == false
    set_param([model_name '/Gain1'],     'Gain',          '1');
else
	set_param([model_name '/Gain1'],     'Gain',          'structArray(1).one');
end
set_param([model_name '/Gain1'], 'OutDataTypeStr', 'double');

set_param([model_name '/Gain2'],     'Gain',          '1');
set_param([model_name '/Gain2'], 'OutDataTypeStr', 'uint32');

% in/out ports
set_param([model_name '/In1_ScalarDouble'], 'IconDisplay',    'Signal name');
set_param([model_name '/In1_ScalarDouble'], 'OutDataTypeStr', 'double');

set_param([model_name '/In2_ScalarUint32'],  'IconDisplay',    'Signal name');
set_param([model_name '/In2_ScalarUint32'],  'OutDataTypeStr', 'uint32');

set_param([model_name '/Out1_ScalarDouble'], 'IconDisplay',    'Signal name');
set_param([model_name '/Out1_ScalarDouble'], 'OutDataTypeStr', 'double');

set_param([model_name '/Out2_ScalarUint32'], 'IconDisplay',    'Signal name');
set_param([model_name '/Out2_ScalarUint32'], 'OutDataTypeStr', 'uint32');

%% manage connections

add_line(model_name, 'In1_ScalarDouble/1', 'Gain1/1');
add_line(model_name, 'Gain1/1',            'Out1_ScalarDouble/1');
add_line(model_name, 'In2_ScalarUint32/1', 'Gain2/1');
add_line(model_name, 'Gain2/1',            'Out2_ScalarUint32/1');


%% signal managing

% name the signals
name_input_signal([model_name '/Out1_ScalarDouble'], 1, 'Out1_ScalarDouble');
name_input_signal([model_name '/Out2_ScalarUint32'], 1, 'Out2_ScalarUint32');

name_output_signal([model_name '/In1_ScalarDouble'], 1, 'In1_ScalarDouble');
name_output_signal([model_name '/In2_ScalarUint32'], 1, 'In2_ScalarUint32');

%% arranging block layout
% alternatively to setting the position of each block, the system can be
% arranged automatically
Simulink.BlockDiagram.arrangeSystem(model_name);

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
%delete(sprintf('%s.slx',model_name));
delete(sprintf('%s.slxc',model_name));
delete(sprintf('%s.slx.bak',model_name));

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