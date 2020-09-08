function model_compiled = createSimpleTestModel(varargin)

global model_name  model_compiled

warning('off', 'all');

%% settings

% default values
modelComplexity      = 1;
hasAllocFcn          = true;
hasGetmmiFcn         = true;
hasTunableParams     = true;
hasStructParams      = false;
hasStructArrayParams = false;

while ~isempty(varargin)
    
    switch varargin{1}
        
        case 'modelComplexity'
            modelComplexity = varargin{2};
        
        case 'hasAllocFcn'
            hasAllocFcn = varargin{2};
        
        case 'hasGetmmiFcn'
            hasGetmmiFcn = varargin{2};
            
        case 'hasTunableParams'
            hasTunableParams = varargin{2};
        
        case 'hasStructParams'
            hasStructParams = varargin{2};
        
        case 'hasStructArrayParams'
            hasStructArrayParams = varargin{2};
            
        otherwise
            error(['Unexpected option: ' varargin{1}])
    end
    
    varargin(1:2) = [];
end

model_name = ['testModel_' int2str(modelComplexity)  int2str(hasAllocFcn)     int2str(hasGetmmiFcn) ...
                           int2str(hasTunableParams) int2str(hasStructParams) int2str(hasStructArrayParams) ...
             ];

model_compiled = false;

if isfile([model_name '.so'])
    return
end

%% define constants

evalin('base', 'matrixConstant = [1 1 1; 2 2 2; 3 3 3];');
evalin('base', 'vectorConstant = ones(10,1);');

if hasStructParams == true
    
    evalin('base', 'structScalar.one = 1;');
    evalin('base', 'structScalar.nested1.one = 1;');
    evalin('base', 'structScalar.nested1.two = 2;');
    evalin('base', 'structScalar.nested2.one = 1;');
    evalin('base', 'structScalar.nested2.two = 2;');
    
    %evalin('base', 'structMixed.one = 1;');
    %evalin('base', 'structMixed.vec = ones(10, 1);');
    
end

if hasStructArrayParams == true
    
    evalin('base', 'structParamArray(1).one = 1;');
    evalin('base', 'structParamArray(1).two = 2;');
    evalin('base', 'structParamArray(2).one = 10;');
    evalin('base', 'structParamArray(2).two = 20;');
    
    evalin('base',  'structMixed.structParamArray(1).one = 1;');
    evalin('base',  'structMixed.structParamArray(1).two = 1;');
    evalin('base',  'structMixed.structParamArray(2).one = 1;');
    evalin('base',  'structMixed.structParamArray(2).two = 1;');
    
end

%% creating a new model

% delete model if it already exists
warning('off','MATLAB:DELETE:FileNotFound');
delete(sprintf('%s.slx',model_name));

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

if hasStructParams == false && hasStructArrayParams == false
    gain1Param = '1';
    gain2Param = '1';
elseif hasStructParams == true && hasStructArrayParams == false
	gain1Param = 'structScalar.nested1.one';
    gain2Param = 'structScalar.nested2.two';
elseif hasStructParams == false && hasStructArrayParams == true
	gain1Param = 'structParamArray(1).one';
    gain2Param = '1';
elseif hasStructParams == true && hasStructArrayParams == true
    gain1Param = 'structScalar.one';
    gain2Param = 'structMixed.structParamArray(1).one';
end

% math blocks
set_param([model_name '/Gain1'], 'Gain',           gain1Param);
set_param([model_name '/Gain1'], 'OutDataTypeStr', 'double');

set_param([model_name '/Gain2'], 'Gain',           gain2Param);
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
set_param(model_name, 'RTWVerbose', 0);

% Optimization
if hasTunableParams == true
    set_param(model_name, 'DefaultParameterBehavior', 'Tunable');
else
    set_param(model_name, 'DefaultParameterBehavior', 'Inlined');
end
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

% Templates
set_param(model_name, 'GenerateSampleERTMain', 0);

    
%% save and close
save_system(model_name);
close_system(model_name);

% clean build directory
rmdir('slprj', 's');
rmdir([model_name '_ert_shrlib_rtw'], 's');
delete(sprintf('%s.slx',model_name));
delete(sprintf('%s.slxc',model_name));
delete(sprintf('%s.slx.bak',model_name));

% 
warning('on','MATLAB:DELETE:FileNotFound');
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