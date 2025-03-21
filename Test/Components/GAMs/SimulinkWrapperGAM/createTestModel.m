function [model_compiled, model_name] = createTestModel(varargin)

evalin('base', 'clear matrixConstant vectorConstant structScalar structMixed');

model_compiled = false;

%% settings

% default values
modelComplexity      = 1;
hasAllocFcn          = true;
hasGetmmiFcn         = true;
hasTunableParams     = false;
hasStructParams      = false;
hasStructArrayParams = false;
hasInputs            = true;
hasOutputs           = true;
hasLoggingSignals    = false;
hasStructSignals     = false;
hasEnums             = false;
dataOrientation      = 'Column-major';   isRowMajor = 0;
useType              = 0;

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

        case 'hasInputs'
            hasInputs = varargin{2};

        case 'hasOutputs'
            hasOutputs = varargin{2};

        case 'hasLoggingSignals'
            hasLoggingSignals = varargin{2};

        case 'hasStructSignals'
            hasStructSignals = varargin{2};

        case 'hasEnums'
            hasEnums = varargin{2};

        case 'dataOrientation'
            dataOrientation = varargin{2};
            if strcmp(dataOrientation, 'Row-major')
                isRowMajor = 1;
            end

        case 'useType'
            useType = varargin{2};

        otherwise
            error(['Unexpected option: ' varargin{1}])
    end

    varargin(1:2) = [];
end

hasStructInputs = ((hasStructSignals == true) && (hasInputs == true));
if verLessThan('matlab', '9.9')
    hasLoggingSignals = false;
    warning('This version does not support configuring logging signals programmatically. hasLoggingSignals set to false.');
end

if hasLoggingSignals && hasEnums
    warning('Enum signals cannot be logged, turning off logging.')
    hasLoggingSignals = false;
end

% warning: the model name is limited to 20 characters
model_name = ['testMdl' int2str(modelComplexity)  int2str(hasAllocFcn)     int2str(hasGetmmiFcn) ...
                        int2str(hasTunableParams) int2str(hasStructParams) int2str(hasStructArrayParams) ...
                        int2str(hasInputs)        int2str(hasOutputs)      int2str(hasStructSignals) ...
                        int2str(isRowMajor)       int2str(hasEnums)        int2str(useType) ...
                        int2str(hasLoggingSignals) ...
             ];

if isfile([model_name '.so'])
    return
end

warning('on', 'verbose');
warning('off', 'Coder:buildProcess:TerminateFunctionNecessaryRequiredOff');
warning('off', 'Simulink:blocks:BusCC_ElementNameMismatchWarning');
if ~hasOutputs
    % disable warning when all signals end in Terminator blocks
    warning('off', 'Simulink:Engine:NoNonVirtualBlocksInModel');
end

%% define handle arrays

outPorts = [];
logBlocks = [];   % blocks whose output is to be logged

%% define constants

if hasTunableParams == true
    evalin('base', 'matrixConstant = [1 1 1; 2 2 2; 3 3 3];');
    evalin('base', 'vectorConstant = uint32(ones(10,1));');

    if modelComplexity >= 2
        evalin('base', 'vectorConstant2 = ones(8,1);');
        if modelComplexity >= 3
            evalin('base', 'matrixConstant2 = ones(6,6);');
            if modelComplexity >= 4
                evalin('base', 'matrixConstant3d = ones(4,4,4);');
            end
        end
    end
end

if hasStructParams == true

    evalin('base', 'structScalar.one = 1;');
    evalin('base', 'structScalar.pnested1.one = 1;');
    evalin('base', 'structScalar.pnested1.two = 2;');
    evalin('base', 'structScalar.pnested2.one = 1;');
    evalin('base', 'structScalar.pnested2.two = 2;');

    if modelComplexity >= 2
        evalin('base', 'structMixed.one = 1;');
        evalin('base', 'structMixed.vec = ones(8, 1)*2;');
        if modelComplexity >= 3
            evalin('base', 'structMixed.mat = uint32(ones(6, 6)*2);');
            if modelComplexity >= 4
                evalin('base', 'structMixed.mat3d = uint32(ones(4, 4, 4)*2);');
            end
        end
    end

end

if hasStructArrayParams == true

    evalin('base', 'structParamArray(1).one = 1;');
    evalin('base', 'structParamArray(1).two = 2;');
    evalin('base', 'structParamArray(2).one = 10;');
    evalin('base', 'structParamArray(2).two = 20;');

    evalin('base',  'structMixed.structParamArray(1).one = uint32(1);');
    evalin('base',  'structMixed.structParamArray(1).two = 1;');
    evalin('base',  'structMixed.structParamArray(2).one = uint32(1);');
    evalin('base',  'structMixed.structParamArray(2).two = 1;');

    if modelComplexity >= 2
        evalin('base', 'paramMatrix(1,1).one = 1;');
        evalin('base', 'paramMatrix(1,1).two = uint32(2*ones(3,4));');
        evalin('base', 'paramMatrix(1,2).one = 10;');
        evalin('base', 'paramMatrix(1,2).two = uint32(20*ones(3,4));');
        evalin('base', 'paramMatrix(1,3).one = 100;');
        evalin('base', 'paramMatrix(1,3).two = uint32(200*ones(3,4));');
        evalin('base', 'paramMatrix(2,1).one = 3;');
        evalin('base', 'paramMatrix(2,1).two = uint32(4*ones(3,4));');
        evalin('base', 'paramMatrix(2,2).one = 30;');
        evalin('base', 'paramMatrix(2,2).two = uint32(40*ones(3,4));');
        evalin('base', 'paramMatrix(2,3).one = 300;');
        evalin('base', 'paramMatrix(2,3).two = uint32(400*ones(3,4));');

        evalin('base', 'structMixed.structParamMatrix(1,1).one = 1;');
        evalin('base', 'structMixed.structParamMatrix(1,1).two = uint32(2*ones(3,4));');
        evalin('base', 'structMixed.structParamMatrix(1,2).one = 10;');
        evalin('base', 'structMixed.structParamMatrix(1,2).two = uint32(20*ones(3,4));');
        evalin('base', 'structMixed.structParamMatrix(1,3).one = 100;');
        evalin('base', 'structMixed.structParamMatrix(1,3).two = uint32(200*ones(3,4));');
        evalin('base', 'structMixed.structParamMatrix(2,1).one = 3;');
        evalin('base', 'structMixed.structParamMatrix(2,1).two = uint32(4*ones(3,4));');
        evalin('base', 'structMixed.structParamMatrix(2,2).one = 30;');
        evalin('base', 'structMixed.structParamMatrix(2,2).two = uint32(40*ones(3,4));');
        evalin('base', 'structMixed.structParamMatrix(2,3).one = 300;');
        evalin('base', 'structMixed.structParamMatrix(2,3).two = uint32(400*ones(3,4));');
    end
end

if hasEnums == true
    Simulink.defineIntEnumType('TestEnum', ...
        {'Off', 'On'}, ...
        [0    ; 1   ], ...
        'StorageType' , 'int32');

    if hasTunableParams == true
        evalin('base', 'EnumParam = TestEnum.Off;');
    end
end

%% creating a new model

% delete model if it already exists
warning('off','MATLAB:DELETE:FileNotFound');
delete(sprintf('%s.slx',model_name));

% create the new model
new_system(model_name);

%% code generation options
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

% Custom code (MODEL is a coder macro for the model name)
if hasGetmmiFcn == true
    set_param(model_name, 'CustomSourceCode', ...
        [  ...
        '#define UTSTRUCTNAME(NAME) RT_MODEL_##NAME##_T'                         newline ...
        '#define TSTRUCTNAME(NAME) UTSTRUCTNAME(NAME)'                           newline ...
        '#define UGETMMIFCN(NAME) NAME##_GetCAPImmi'                             newline ...
        '#define GETMMIFCN(NAME) UGETMMIFCN(NAME)'                               newline ...
                                                                                 newline ...
        'rtwCAPI_ModelMappingInfo* GETMMIFCN(MODEL) ( TSTRUCTNAME(MODEL) *rtm )' newline ...
        '{'                                                                      newline ...
        '    return &(rtmGetDataMapInfo(rtm).mmi);'                              newline ...
        '}'                                                                      newline ...
        ] ...
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
if hasLoggingSignals == true
    set_param(model_name, 'RTWCAPISignals', 1);
    set_param(model_name, 'SignalLogging', 'on');
end

if hasAllocFcn == true
    set_param(model_name, 'GenerateAllocFcn', 1);
end

set_param(model_name, 'IncludeMdlTerminateFcn', 0);
set_param(model_name, 'CombineSignalStateStructs', 1);

if ~verLessThan('matlab', '9.5')
    set_param(model_name, 'ArrayLayout', dataOrientation);
end

% Templates
set_param(model_name, 'GenerateSampleERTMain', 0);

save_system(model_name);

%% adding blocks

% -- root system

set_param(model_name, 'TunableVars', 'paramMatrix, structMixed');
set_param(model_name, 'TunableVarsStorageclass', 'Auto,Auto');
set_param(model_name, 'TunableVarsTypeQualifier', ',')

% math blocks
add_block('simulink/Math Operations/Gain', [model_name '/Gain1']);
add_block('simulink/Math Operations/Gain', [model_name '/Gain2']);

if modelComplexity >= 2
    add_block('simulink/Math Operations/Gain', [model_name '/Gain3'], ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'Inherit: Same as input');
    add_block('simulink/Math Operations/Gain', [model_name '/Gain4'], ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'Inherit: Same as input');
    if hasStructParams == true
        set_param([model_name '/Gain3'], 'Gain', 'structMixed.one');
    end
end

if modelComplexity >= 3
    add_block('simulink/Math Operations/Gain', [model_name '/Gain5'], ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'Inherit: Same as input');
    add_block('simulink/Math Operations/Gain', [model_name '/Gain6'], ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'Inherit: Same as input');
end

% input ports
inPorts = helper_input_gen(model_name, hasInputs, hasStructInputs, modelComplexity, hasTunableParams, hasStructParams, hasStructArrayParams, hasEnums);

% output ports
if hasOutputs == true
    outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out1_ScalarDouble'], ...
        'SignalName',     'Out1_ScalarDouble', ...
        'IconDisplay',    'Signal name', ...
        'OutDataTypeStr', 'double');

    outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out2_ScalarUint32'], ...
        'SignalName',     'Out2_ScalarUint32', ...
        'IconDisplay',    'Signal name');
    if hasEnums == false
        set_param([model_name '/Out2_ScalarUint32'], 'OutDataTypeStr', 'uint32');
    else
        set_param([model_name '/Out2_ScalarUint32'], 'OutDataTypeStr', 'Enum:TestEnum');
    end

    if modelComplexity >= 2
        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out3_VectorDouble'], ...
            'SignalName',     'Out3_VectorDouble', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'double');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out4_VectorUint32'], ...
            'SignalName',     'Out4_VectorUint32', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'uint32');
    end

    if modelComplexity >= 3
        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out5_MatrixDouble'], ...
            'SignalName',     'Out5_MatrixDouble', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'double');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out6_MatrixUint32'], ...
            'SignalName',     'Out6_MatrixUint32', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'uint32');
    end

    if modelComplexity >= 4
        % MARTe does not support 3D signals, so the ports output only a page of the 3D signal
        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out7_3DMatrixDouble'], ...
            'SignalName',     'Out7_3DMatrixDouble', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'double');

        % selector to select the output page
        add_block('simulink/Signal Routing/Selector', [model_name '/SelectorDouble'],...
            'NumberOfDimensions', '3', 'IndexOptions', 'Index vector (dialog),Index vector (dialog),Index vector (dialog)',...
            'Indices',           '[1:4],[1:4],2');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out8_3DMatrixUint32'], ...
            'SignalName',     'Out8_3DMatrixUint32', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Selector', [model_name '/Selector3DUint32'],...
            'NumberOfDimensions', '3', 'IndexOptions', 'Index vector (dialog),Index vector (dialog),Index vector (dialog)',...
            'Indices',           '[1:4],[1:4],2');
    end

else
    add_block('simulink/Sinks/Terminator', [model_name '/Out1_ScalarDouble']);
    add_block('simulink/Sinks/Terminator', [model_name '/Out2_ScalarUint32']);
    if modelComplexity >= 2
        add_block('simulink/Sinks/Terminator',  [model_name '/Out3_VectorDouble']);
        add_block('simulink/Sinks/Terminator',  [model_name '/Out4_VectorUint32']);
        if modelComplexity >= 3
           add_block('simulink/Sinks/Terminator',  [model_name '/Out5_MatrixDouble']);
           add_block('simulink/Sinks/Terminator',  [model_name '/Out6_MatrixUint32']);
        end
        if modelComplexity >= 4
           add_block('simulink/Sinks/Terminator',  [model_name '/Out7_3DMatrixDouble']);
           add_block('simulink/Sinks/Terminator',  [model_name '/Out8_3DMatrixUint32']);
        end
    end
end

if hasStructSignals == true

    % custom type for the nonvirtual bus
    evalin('base', 'clear bus1Elems;');
    evalin('base', 'bus1Elems(1) = Simulink.BusElement;');
    evalin('base', 'bus1Elems(1).Name = ''Signal1'';');
    evalin('base', 'bus1Elems(1).Dimensions = 1;');
    evalin('base', 'bus1Elems(1).DimensionsMode = ''Fixed'';');
    evalin('base', 'bus1Elems(1).DataType = ''uint32'';');
    evalin('base', 'bus1Elems(1).SampleTime = -1;');
    evalin('base', 'bus1Elems(1).Complexity = ''real'';');

    evalin('base', 'bus1Elems(2) = Simulink.BusElement;');
    evalin('base', 'bus1Elems(2).Name = ''Signal2'';');
    evalin('base', 'bus1Elems(2).Dimensions = 1;');
    evalin('base', 'bus1Elems(2).DimensionsMode = ''Fixed'';');
    evalin('base', 'bus1Elems(2).DataType = ''double'';');
    evalin('base', 'bus1Elems(2).SampleTime = -1;');
    evalin('base', 'bus1Elems(2).Complexity = ''real'';');

    evalin('base', 'STRUCTSIGNAL1 = Simulink.Bus;');
    evalin('base', 'STRUCTSIGNAL1.Elements = bus1Elems;');

    add_block('simulink/Signal Routing/Bus Creator', [model_name '/BusCreator1']);
    set_param([model_name '/BusCreator1'],         'Inputs',         '2');
    set_param([model_name '/BusCreator1'],         'NonVirtualBus',  'on');
    set_param([model_name '/BusCreator1'],         'OutDataTypeStr', 'Bus: STRUCTSIGNAL1');

    outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out20_NonVirtualBus'], ...
        'SignalName',     'Out20_NonVirtualBus', ...
        'IconDisplay',    'Signal name', ...
        'OutDataTypeStr', 'Inherit: auto');

    if(modelComplexity > 1)
        evalin('base', 'clear bus3Elems;');
        evalin('base', 'bus21Elems(1) = Simulink.BusElement;');
        evalin('base', 'bus21Elems(1).Name = ''Signal1'';');
        evalin('base', 'bus21Elems(1).Dimensions = [8 1];');
        evalin('base', 'bus21Elems(1).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus21Elems(1).DataType = ''uint32'';');
        evalin('base', 'bus21Elems(1).SampleTime = -1;');
        evalin('base', 'bus21Elems(1).Complexity = ''real'';');

        evalin('base', 'bus21Elems(2) = Simulink.BusElement;');
        evalin('base', 'bus21Elems(2).Name = ''Signal2'';');
        evalin('base', 'bus21Elems(2).Dimensions = [8 1];');
        evalin('base', 'bus21Elems(2).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus21Elems(2).DataType = ''double'';');
        evalin('base', 'bus21Elems(2).SampleTime = -1;');
        evalin('base', 'bus21Elems(2).Complexity = ''real'';');

        evalin('base', 'STRUCTSIGNAL21 = Simulink.Bus;');
        evalin('base', 'STRUCTSIGNAL21.Elements = bus21Elems;');

        add_block('simulink/Signal Routing/Bus Creator', [model_name '/BusCreator21']);
        set_param([model_name '/BusCreator21'],         'Inputs',         '2');
        set_param([model_name '/BusCreator21'],         'NonVirtualBus',  'on');
        set_param([model_name '/BusCreator21'],         'OutDataTypeStr', 'Bus: STRUCTSIGNAL21');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out21_NonVirtualBus'], ...
            'SignalName',     'Out21_NonVirtualBus', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Inherit: auto');
    end

    if(modelComplexity > 2)
        evalin('base', 'clear bus31Elems;');
        evalin('base', 'bus31Elems(1) = Simulink.BusElement;');
        evalin('base', 'bus31Elems(1).Name = ''Signal1'';');
        evalin('base', 'bus31Elems(1).Dimensions = [6 6];');
        evalin('base', 'bus31Elems(1).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus31Elems(1).DataType = ''uint32'';');
        evalin('base', 'bus31Elems(1).SampleTime = -1;');
        evalin('base', 'bus31Elems(1).Complexity = ''real'';');

        evalin('base', 'bus31Elems(2) = Simulink.BusElement;');
        evalin('base', 'bus31Elems(2).Name = ''Signal2'';');
        evalin('base', 'bus31Elems(2).Dimensions = [6 6];');
        evalin('base', 'bus31Elems(2).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus31Elems(2).DataType = ''double'';');
        evalin('base', 'bus31Elems(2).SampleTime = -1;');
        evalin('base', 'bus31Elems(2).Complexity = ''real'';');

        evalin('base', 'STRUCTSIGNAL31 = Simulink.Bus;');
        evalin('base', 'STRUCTSIGNAL31.Elements = bus31Elems;');

        add_block('simulink/Signal Routing/Bus Creator', [model_name '/BusCreator31']);
        set_param([model_name '/BusCreator31'],         'Inputs',         '2');
        set_param([model_name '/BusCreator31'],         'NonVirtualBus',  'on');
        set_param([model_name '/BusCreator31'],         'OutDataTypeStr', 'Bus: STRUCTSIGNAL31');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out31_NonVirtualBus'], ...
            'SignalName',     'Out31_NonVirtualBus', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Inherit: auto');

    end

    if modelComplexity > 3
        evalin('base', 'clear bus2031Elems;');
        evalin('base', 'bus2031Elems(1) = Simulink.BusElement;');
        evalin('base', 'bus2031Elems(1).Name = ''Signal1'';');
        evalin('base', 'bus2031Elems(1).Dimensions = 1;');
        evalin('base', 'bus2031Elems(1).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus2031Elems(1).DataType = ''Bus: STRUCTSIGNAL21'';');
        evalin('base', 'bus2031Elems(1).SampleTime = -1;');
        evalin('base', 'bus2031Elems(1).Complexity = ''real'';');

        evalin('base', 'bus2031Elems(2) = Simulink.BusElement;');
        evalin('base', 'bus2031Elems(2).Name = ''Signal2'';');
        evalin('base', 'bus2031Elems(2).Dimensions = 1;');
        evalin('base', 'bus2031Elems(2).DimensionsMode = ''Fixed'';');
        evalin('base', 'bus2031Elems(2).DataType = ''Bus: STRUCTSIGNAL31'';');
        evalin('base', 'bus2031Elems(2).SampleTime = -1;');
        evalin('base', 'bus2031Elems(2).Complexity = ''real'';');

        evalin('base', 'STRUCTSIGNAL2031 = Simulink.Bus;');
        evalin('base', 'STRUCTSIGNAL2031.Elements = bus2031Elems;');

        add_block('simulink/Signal Routing/Bus Creator', [model_name '/BusCreator2031']);
            set_param([model_name '/BusCreator2031'],         'Inputs',         '2');
            set_param([model_name '/BusCreator2031'],         'NonVirtualBus',  'on');
            set_param([model_name '/BusCreator2031'],         'OutDataTypeStr', 'Bus: STRUCTSIGNAL2031');

        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out2031_NonVirtualBus'], ...
            'SignalName',     'Out2031_NonVirtualBus', ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Inherit: auto');

        add_line(model_name, 'BusCreator21/1',            'BusCreator2031/1');
        add_line(model_name, 'BusCreator31/1',            'BusCreator2031/2');
        add_line(model_name, 'BusCreator2031/1',          'Out2031_NonVirtualBus/1');

        name_output_signal([model_name '/Out2031_NonVirtualBus'], 1, 'Out2031_NonVirtualBus');
    end

end

%% manage connections

% connect outputs
add_line(model_name, 'Gain1/1',            'Out1_ScalarDouble/1');
if hasEnums == true
    % gain block does not support enums
    delete_block([model_name '/Gain2']);
    add_block('simulink/Signal Attributes/Signal Specification', [model_name '/Gain2' ], ...
            'OutDataTypeStr', 'Enum:TestEnum');
end
add_line(model_name, 'Gain2/1',            'Out2_ScalarUint32/1');

if modelComplexity >= 2
    add_line(model_name, 'Gain3/1',            'Out3_VectorDouble/1');
    add_line(model_name, 'Gain4/1',            'Out4_VectorUint32/1');
end

if modelComplexity >= 3
    add_line(model_name, 'Gain5/1',            'Out5_MatrixDouble/1');
    add_line(model_name, 'Gain6/1',            'Out6_MatrixUint32/1');
end

if hasStructSignals == true
    add_line(model_name, 'Gain1/1',            'BusCreator1/2');
    add_line(model_name, 'Gain2/1',            'BusCreator1/1');

    add_line(model_name, 'BusCreator1/1',      'Out20_NonVirtualBus/1');

    if modelComplexity >= 2
        add_line(model_name, 'BusCreator21/1',      'Out21_NonVirtualBus/1');
        add_line(model_name, 'Gain3/1',            'BusCreator21/2');
        add_line(model_name, 'Gain4/1',            'BusCreator21/1');
    end

    if modelComplexity >= 3
        add_line(model_name, 'BusCreator31/1',      'Out31_NonVirtualBus/1');
        add_line(model_name, 'Gain5/1',            'BusCreator31/2');
        add_line(model_name, 'Gain6/1',            'BusCreator31/1');
    end

end

% insert more gains to generate a signal to be logged
if hasLoggingSignals
    set_param([model_name, '/Gain1'], 'Name', 'Gain1_ForLogging', 'OutDataTypeStr', 'double');
    set_param([model_name, '/Gain2'], 'Name', 'Gain2_ForLogging', 'OutDataTypeStr', 'uint32');
    logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain1'], 'OutDataTypeStr', 'double');
    if hasEnums
        %gain block does not support enums
        logBlocks(end + 1) = add_block('simulink/Signal Attributes/Signal Specification', [model_name '/Gain2'], ...
            'OutDataTypeStr', 'Enum:TestEnum');
        set_param([model_name, '/Gain2_ForLogging'], 'OutDataTypeStr', 'Enum:TestEnum');
    else
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain2'], ...
            'OutDataTypeStr', 'uint32');
    end
    l1 = add_line(model_name, 'Gain1/1', 'Gain1_ForLogging/1');
    l2 = add_line(model_name, 'Gain2/1', 'Gain2_ForLogging/1');
    set_param(l1, 'Name', 'Log1');
    set_param(l2, 'Name', 'Log2');

    if modelComplexity >= 2
        set_param([model_name, '/Gain3'], 'Name', 'Gain3_ForLogging', 'OutDataTypeStr', 'double');
        set_param([model_name, '/Gain4'], 'Name', 'Gain4_ForLogging', 'OutDataTypeStr', 'uint32');
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain3'], 'OutDataTypeStr', 'double');
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain4'], 'OutDataTypeStr', 'uint32');
        l1 = add_line(model_name, 'Gain3/1', 'Gain3_ForLogging/1');
        l2 = add_line(model_name, 'Gain4/1', 'Gain4_ForLogging/1');
        set_param(l1, 'Name', 'Log3');
        set_param(l2, 'Name', 'Log4');
    end
    
    if modelComplexity >= 3
        set_param([model_name, '/Gain5'], 'Name', 'Gain5_ForLogging', 'OutDataTypeStr', 'double');
        set_param([model_name, '/Gain6'], 'Name', 'Gain6_ForLogging', 'OutDataTypeStr', 'uint32');
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain5'], 'OutDataTypeStr', 'double');
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', [model_name '/Gain6'], 'OutDataTypeStr', 'uint32');
        l1 = add_line(model_name, 'Gain5/1', 'Gain5_ForLogging/1');
        l2 = add_line(model_name, 'Gain6/1', 'Gain6_ForLogging/1');
        set_param(l1, 'Name', 'Log5');
        set_param(l2, 'Name', 'Log6');
    end
end

% connect inputs
add_line(model_name, 'In1_ScalarDouble/1', 'Gain1/1');
add_line(model_name, 'In2_ScalarUint32/1', 'Gain2/1');

if modelComplexity >= 2
    add_line(model_name, 'In3_VectorDouble/1', 'Gain3/1')
    add_line(model_name, 'In4_VectorUint32/1', 'Gain4/1')
end

if modelComplexity >= 3
    add_line(model_name, 'In5_MatrixDouble/1', 'Gain5/1')
    add_line(model_name, 'In6_MatrixUint32/1', 'Gain6/1')
end

if modelComplexity >= 4
    if hasInputs == true
        add_line(model_name, 'In7_3DMatrixDouble/1', 'Concatenate7/1');
        add_line(model_name, 'In7_3DMatrixDouble/1', 'Concatenate7/2');
        add_line(model_name, 'In8_3DMatrixUint32/1', 'Concatenate8/1');
        add_line(model_name, 'In8_3DMatrixUint32/1', 'Concatenate8/2');

        add_line(model_name, 'Concatenate7/1', 'SelectorDouble/1');
        add_line(model_name, 'Concatenate8/1', 'Selector3DUint32/1');
    else
        add_line(model_name, 'In7_3DMatrixDouble/1', 'SelectorDouble/1');
        add_line(model_name, 'In8_3DMatrixUint32/1', 'Selector3DUint32/1');
    end

    add_line(model_name, 'SelectorDouble/1', 'Out7_3DMatrixDouble/1');
    add_line(model_name, 'Selector3DUint32/1', 'Out8_3DMatrixUint32/1');
end

%% set block properties

gain1Param = '1';
gain2Param = '2';

if hasStructParams == false && hasStructArrayParams == false && hasTunableParams == false
    gain1Param = '1';
    gain2Param = '1';
elseif hasStructParams == false && hasStructArrayParams == false && hasTunableParams == true
    gain1Param = '1';
    gain2Param = 'vectorConstant';
elseif hasStructParams == true && hasStructArrayParams == false && hasTunableParams == true
    gain1Param = 'structScalar.pnested1.one';
    gain2Param = 'structScalar.pnested2.two';
elseif hasStructParams == false && hasStructArrayParams == true && hasTunableParams == true
    gain1Param = 'structParamArray(1).one';
    gain2Param = '1';
elseif hasStructParams == true && hasStructArrayParams == true && hasTunableParams == true
    if modelComplexity == 1
        gain1Param = 'structParamArray(1).one';
        gain2Param = 'structMixed.structParamArray(1).one';
    elseif modelComplexity >= 2
        gain1Param = 'paramMatrix(2,2).one';
        gain2Param = 'structMixed.structParamMatrix(2,2).one';
    end
end

% math blocks
set_param([model_name '/Gain1'], ...
    'Gain',           gain1Param, ...
    'OutDataTypeStr', 'double');

if ~hasEnums
    set_param([model_name '/Gain2'], 'Gain',           gain2Param);
    set_param([model_name '/Gain2'], 'OutDataTypeStr',   'uint32');
    set_param([model_name '/Gain2'], 'ParamDataTypeStr', 'uint32');
end

% change types if requested
if useType == 1

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'uint8');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'uint16');
        evalin('base', 'matrixConstant2 = uint8(ones(6,6));');
        evalin('base', 'structMixed.mat = uint16(ones(6, 6)*2);');
        if hasOutputs == true
            set_param([model_name '/Out5_MatrixDouble'],   'OutDataTypeStr', 'uint8');
            set_param([model_name '/Out6_MatrixUint32'],   'OutDataTypeStr', 'uint16');
        end
        if hasLoggingSignals
            set_param([model_name '/Gain5_ForLogging'], 'OutDataTypeStr',   'uint8');
            set_param([model_name '/Gain6_ForLogging'], 'OutDataTypeStr',   'uint16');
        end
    end
    if modelComplexity >= 4
        set_param([model_name '/In7_3DMatrixDouble'], 'OutDataTypeStr', 'uint32');
        %set_param([model_name '/In8_3DMatrixUint32'], 'OutDataTypeStr', 'uint64'); % uint64 not yet supported by coder
        if hasOutputs == true
            set_param([model_name '/Out7_3DMatrixDouble'], 'OutDataTypeStr', 'uint32');
            %set_param([model_name '/Out8_3DMatrixUint32'], 'OutDataTypeStr', 'uint64'); % uint64 not yet supported by coder
        end
    end

end

if useType == 2

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'int8');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'int16');
        evalin('base', 'matrixConstant2 = int8(ones(6,6));');
        evalin('base', 'structMixed.mat = int16(ones(6, 6)*2);');
        if hasOutputs == true
            set_param([model_name '/Out5_MatrixDouble'],   'OutDataTypeStr', 'int8');
            set_param([model_name '/Out6_MatrixUint32'],   'OutDataTypeStr', 'int16');
        end
        if hasLoggingSignals
            set_param([model_name '/Gain5_ForLogging'], 'OutDataTypeStr',   'int8');
            set_param([model_name '/Gain6_ForLogging'], 'OutDataTypeStr',   'int16');
        end
    end
    if modelComplexity >= 4
        set_param([model_name '/In7_3DMatrixDouble'], 'OutDataTypeStr', 'int32');
        %set_param([model_name '/In8_3DMatrixUint32'], 'OutDataTypeStr', 'int64'); % int64 not yet supported by coder
        if hasOutputs == true
            set_param([model_name '/Out7_3DMatrixDouble'], 'OutDataTypeStr', 'int32');
            %set_param([model_name '/Out8_3DMatrixUint32'], 'OutDataTypeStr', 'int64'); % int64 not yet supported by coder
        end
    end

end

if useType == 4

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'int32');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'single');
        evalin('base', 'matrixConstant2 = int32(ones(6,6));');
        evalin('base', 'structMixed.mat = single(ones(6, 6)*2);');
        if hasOutputs == true
            set_param([model_name '/Out5_MatrixDouble'],   'OutDataTypeStr', 'int32');
            set_param([model_name '/Out6_MatrixUint32'],   'OutDataTypeStr', 'single');
        end
        if hasLoggingSignals
            set_param([model_name '/Gain5_ForLogging'], 'OutDataTypeStr',   'int32');
            set_param([model_name '/Gain6_ForLogging'], 'OutDataTypeStr',   'single');
        end
    end
    if modelComplexity >= 4
        set_param([model_name '/In7_3DMatrixDouble'], 'OutDataTypeStr', 'int32');
        set_param([model_name '/In8_3DMatrixUint32'], 'OutDataTypeStr', 'single');
        if hasOutputs == true
            set_param([model_name '/Out7_3DMatrixDouble'], 'OutDataTypeStr', 'int32');
            set_param([model_name '/Out8_3DMatrixUint32'], 'OutDataTypeStr', 'single');
        end
    end

end

%% signal naming

% name the output signals
for port = outPorts
    p           = get_param(port, 'PortHandles');
    signal_name = get_param(port, 'Name');
    l           = get_param(p.Inport(1), 'Line');
    set_param(l, 'Name', signal_name);
end

% name the input signals
for port = inPorts
    p           = get_param(port, 'PortHandles');
    signal_name = get_param(port, 'Name');
    l           = get_param(p.Outport(1), 'Line');
    set_param(l, 'Name', signal_name);
end

%% logging of block signals

if hasLoggingSignals
    try
        cm = coder.mapping.api.get(model_name);
    catch
        cm = coder.mapping.utils.create(model_name);
    end
    setDataDefault(cm, "InternalData", "StorageClass", "MultiInstance");

    for block = logBlocks
        p = get_param(block, 'PortHandles');
        l = p.Outport(1);
        addSignal(cm, l, "StorageClass", "Model default");
    end
end

%% arranging block layout
% alternatively to setting the position of each block, the system can be
% arranged automatically
if ~verLessThan('matlab', '9.5')
    Simulink.BlockDiagram.arrangeSystem(model_name);
end

%% model build

try
    rtwbuild(model_name)
    model_compiled = true;
catch ME
    model_compiled = false;
    warning('on', ME.identifier);
    warning(ME.identifier, '%s', ME.message)
end


%% save and close
save_system(model_name);
close_system(model_name);

% clean build directory
try
    rmdir('slprj', 's');
    rmdir([model_name '_ert_shrlib_rtw'], 's');
catch ME
    warning(ME.identifier, '%s', ME.message)
end

delete(sprintf('%s.slx',model_name));
delete(sprintf('%s.slxc',model_name));
delete(sprintf('%s.slx.bak',model_name));

%
warning('on', 'all');

end   % function

%% functions

%Helper functions, just to keep main code flow tidy, block-structured and
%easier to focus on

function tempInPorts = helper_input_gen_WithInputs(model_name, modelComplexity, hasEnums)

    tempInPorts = [];

    tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In1_ScalarDouble'], ...
        'IconDisplay',    'Signal name', ...
        'OutDataTypeStr', 'double');

    tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In2_ScalarUint32' ], ...
        'IconDisplay',    'Signal name');
    if hasEnums == false
        set_param([model_name '/In2_ScalarUint32'],  'OutDataTypeStr', 'uint32');
    else
        set_param([model_name '/In2_ScalarUint32'],  'OutDataTypeStr', 'Enum:TestEnum');
    end

    if modelComplexity >= 2
        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In3_VectorDouble' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'double',      ...
            'PortDimensions', '[8 1]');

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In4_VectorUint32' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'uint32',      ...
            'PortDimensions', '[8 1]' );
    end

    if modelComplexity >= 3
        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In5_MatrixDouble' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'double',      ...
            'PortDimensions', '[6 6]' );

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In6_MatrixUint32' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'uint32',      ...
            'PortDimensions', '[6 6]' );
    end

    if modelComplexity >= 4
        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In7_3DMatrixDouble' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'double', ...
            'PortDimensions',  '[4 4]');

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In8_3DMatrixUint32' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'uint32', ...
            'PortDimensions',  '[4 4]');

        add_block('simulink/Math Operations/Matrix Concatenate', [model_name '/Concatenate7'], ...
            'NumInputs',            '2', ...
            'ConcatenateDimension', '3')

        add_block('simulink/Math Operations/Matrix Concatenate', [model_name '/Concatenate8'], ...
            'NumInputs',            '2', ...
            'ConcatenateDimension', '3')
    end
    
end

function tempInPorts = helper_input_gen_WithoutInputs(model_name, modelComplexity, hasTunableParams, hasStructParams, hasStructArrayParams, hasEnums)

    tempInPorts = [];

    tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In1_ScalarDouble'], ...
        'Value',          '1', ...
        'OutDataTypeStr', 'double');

    tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In2_ScalarUint32']);
    if hasEnums == false
        set_param([model_name '/In2_ScalarUint32'], 'Value',          '1');
        set_param([model_name '/In2_ScalarUint32'], 'OutDataTypeStr', 'uint32');
    else
        set_param([model_name '/In2_ScalarUint32'], 'OutDataTypeStr', 'Enum:TestEnum');
        if hasTunableParams == true
            set_param([model_name '/In2_ScalarUint32'], 'Value',          'EnumParam');
        else
            set_param([model_name '/In2_ScalarUint32'], 'Value',          'TestEnum.On');
        end
    end

    if (modelComplexity >= 2)
        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In3_VectorDouble'], ...
            'OutDataTypeStr', 'double');
        if hasTunableParams == true
            set_param([model_name '/In3_VectorDouble'],  'Value',          'vectorConstant2');
        else
            set_param([model_name '/In3_VectorDouble'],  'Value',          'rand(8,1)');
        end

        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In4_VectorUint32'], ...
            'OutDataTypeStr', 'uint32');
        if hasStructParams == true
            set_param([model_name '/In4_VectorUint32'], 'Value',          'uint32(structMixed.vec)');
        else
            set_param([model_name '/In4_VectorUint32'], 'Value',          'ones(8,1)');
        end
    end

    if (modelComplexity >= 3)
        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In5_MatrixDouble'], ...
            'OutDataTypeStr', 'double');
        if hasTunableParams == true
            if hasStructParams == true && hasStructArrayParams == true
                set_param([model_name '/In5_MatrixDouble'],  'Value',          'double(structMixed.structParamMatrix(2,2).two)');
            else
                set_param([model_name '/In5_MatrixDouble'],  'Value',          'matrixConstant2');
            end
        else
            set_param([model_name '/In5_MatrixDouble'], 'Value',          'rand(6,6)');
        end

        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In6_MatrixUint32'], ...
            'OutDataTypeStr', 'uint32');
        if hasTunableParams == true
            if hasStructParams == true && hasStructArrayParams == true
                set_param([model_name '/In6_MatrixUint32'], 'Value',          'paramMatrix(1,2).two');
            elseif hasStructParams == true && hasStructArrayParams == false
                set_param([model_name '/In6_MatrixUint32'], 'Value',          'structMixed.mat');
            else
                set_param([model_name '/In6_MatrixUint32'], 'Value',          'uint32(matrixConstant2)');
            end
        else
            set_param([model_name '/In6_MatrixUint32'],  'Value',          'ones(6,6)');
        end
    end

    if (modelComplexity >= 4)
        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In7_3DMatrixDouble'], ...
            'OutDataTypeStr', 'double');
        if hasTunableParams == true
            set_param([model_name '/In7_3DMatrixDouble'],  'Value',          'matrixConstant3d');
        else
            set_param([model_name '/In7_3DMatrixDouble'], 'Value',          'rand(4,4,4)');
        end

        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In8_3DMatrixUint32'], ...
            'OutDataTypeStr', 'uint32');
        if hasStructParams == true
            set_param([model_name '/In8_3DMatrixUint32'], 'Value',          'structMixed.mat3d');
        else
            set_param([model_name '/In8_3DMatrixUint32'],  'Value',          'ones(4,4,4)');
        end
    end

end

function tempInPorts = helper_input_gen_WithStructInputs(model_name, modelComplexity)

    tempInPorts = [];

    evalin('base', 'clear inputBusElems;');

    evalin('base', 'inputBusElems(1) = Simulink.BusElement;');
    evalin('base', 'inputBusElems(1).Name = ''In2_ScalarUint32'';');
    evalin('base', 'inputBusElems(1).Dimensions = 1;');
    evalin('base', 'inputBusElems(1).DimensionsMode = ''Fixed'';');
    evalin('base', 'inputBusElems(1).DataType = ''uint32'';');
    evalin('base', 'inputBusElems(1).SampleTime = -1;');
    evalin('base', 'inputBusElems(1).Complexity = ''real'';');

    evalin('base', 'inputBusElems(2) = Simulink.BusElement;');
    evalin('base', 'inputBusElems(2).Name = ''In1_ScalarDouble'';');
    evalin('base', 'inputBusElems(2).Dimensions = 1;');
    evalin('base', 'inputBusElems(2).DimensionsMode = ''Fixed'';');
    evalin('base', 'inputBusElems(2).DataType = ''double'';');
    evalin('base', 'inputBusElems(2).SampleTime = -1;');
    evalin('base', 'inputBusElems(2).Complexity = ''real'';');

    evalin('base', 'INSTRUCTSIGNAL1 = Simulink.Bus;');
    evalin('base', 'INSTRUCTSIGNAL1.Elements = inputBusElems;');

    tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In1_Structured'], ...
        'IconDisplay',    'Signal name', ...
        'OutDataTypeStr', 'Bus: INSTRUCTSIGNAL1', ...
        'BusOutputAsStruct',  'on');

    add_block('simulink/Math Operations/Gain', [model_name '/In1_ScalarDouble'], ...
              'OutDataTypeStr', 'double');
    add_block('simulink/Math Operations/Gain', [model_name '/In2_ScalarUint32'], ...
              'OutDataTypeStr', 'uint32');

    add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector'], ...
        'OutputSignals', 'In2_ScalarUint32,In1_ScalarDouble');

    add_line(model_name, 'InputSelector/2', 'In1_ScalarDouble/1');
    add_line(model_name, 'InputSelector/1', 'In2_ScalarUint32/1');

    add_line(model_name, 'In1_Structured/1', 'InputSelector/1');
    name_input_signal([model_name '/In1_Structured'], 1, 'In1_Structured');

    if modelComplexity > 1
        evalin('base', 'clear inputBusElems2;');
        evalin('base', 'inputBusElems2(2) = Simulink.BusElement;');
        evalin('base', 'inputBusElems2(2).Name = ''In3_VectorDouble'';');
        evalin('base', 'inputBusElems2(2).Dimensions = [8 1];');
        evalin('base', 'inputBusElems2(2).DimensionsMode = ''Fixed'';');
        evalin('base', 'inputBusElems2(2).DataType = ''double'';');
        evalin('base', 'inputBusElems2(2).SampleTime = -1;');
        evalin('base', 'inputBusElems2(2).Complexity = ''real'';');

        evalin('base', 'inputBusElems2(1) = Simulink.BusElement;');
        evalin('base', 'inputBusElems2(1).Name = ''In4_VectorUint32'';');
        evalin('base', 'inputBusElems2(1).Dimensions = [8 1];');
        evalin('base', 'inputBusElems2(1).DimensionsMode = ''Fixed'';');
        evalin('base', 'inputBusElems2(1).DataType = ''uint32'';');
        evalin('base', 'inputBusElems2(1).SampleTime = -1;');
        evalin('base', 'inputBusElems2(1).Complexity = ''real'';');

        evalin('base', 'INSTRUCTSIGNAL2 = Simulink.Bus;');
        evalin('base', 'INSTRUCTSIGNAL2.Elements = inputBusElems2;');

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In2_Structured'], ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Bus: INSTRUCTSIGNAL2', ...
            'BusOutputAsStruct',  'on');

        add_block('simulink/Math Operations/Gain', [model_name '/In3_VectorDouble'], ...
            'OutDataTypeStr', 'double');
        add_block('simulink/Math Operations/Gain', [model_name '/In4_VectorUint32'], ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector2'], ...
            'OutputSignals', 'In4_VectorUint32,In3_VectorDouble');

        add_line(model_name, 'InputSelector2/2', 'In3_VectorDouble/1');
        add_line(model_name, 'InputSelector2/1', 'In4_VectorUint32/1');

        add_line(model_name, 'In2_Structured/1', 'InputSelector2/1');
        name_input_signal([model_name '/In2_Structured'], 1, 'In2_Structured');
    end

    if modelComplexity > 2
        evalin('base', 'inputBusElems3(2) = Simulink.BusElement;');
        evalin('base', 'inputBusElems3(2).Name = ''In5_MatrixDouble'';');
        evalin('base', 'inputBusElems3(2).Dimensions = [6 6];');
        evalin('base', 'inputBusElems3(2).DimensionsMode = ''Fixed'';');
        evalin('base', 'inputBusElems3(2).DataType = ''double'';');
        evalin('base', 'inputBusElems3(2).SampleTime = -1;');
        evalin('base', 'inputBusElems3(2).Complexity = ''real'';');

        evalin('base', 'inputBusElems3(1) = Simulink.BusElement;');
        evalin('base', 'inputBusElems3(1).Name = ''In6_MatrixUint32'';');
        evalin('base', 'inputBusElems3(1).Dimensions = [6 6];');
        evalin('base', 'inputBusElems3(1).DimensionsMode = ''Fixed'';');
        evalin('base', 'inputBusElems3(1).DataType = ''uint32'';');
        evalin('base', 'inputBusElems3(1).SampleTime = -1;');
        evalin('base', 'inputBusElems3(1).Complexity = ''real'';');

        evalin('base', 'INSTRUCTSIGNAL3 = Simulink.Bus;');
        evalin('base', 'INSTRUCTSIGNAL3.Elements = inputBusElems3;');

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In3_Structured'], ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Bus: INSTRUCTSIGNAL3', ...
            'BusOutputAsStruct',  'on');

        add_block('simulink/Math Operations/Gain', [model_name '/In5_MatrixDouble'], ...
            'OutDataTypeStr', 'double');
        add_block('simulink/Math Operations/Gain', [model_name '/In6_MatrixUint32'], ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector3'], ...
            'OutputSignals', 'In6_MatrixUint32,In5_MatrixDouble');

        add_line(model_name, 'InputSelector3/2', 'In5_MatrixDouble/1');
        add_line(model_name, 'InputSelector3/1', 'In6_MatrixUint32/1');

        add_line(model_name, 'In3_Structured/1', 'InputSelector3/1');
        name_input_signal([model_name '/In3_Structured'], 1, 'In3_Structured');
    end
end

function inPorts = helper_input_gen(modelName, withInputs, withStructs, modelComplexity, hasTunableParams, hasStructParams, hasStructArrayParams, hasEnums)

    if withInputs == true
       if withStructs == true
           inPorts = helper_input_gen_WithStructInputs(modelName, modelComplexity);
       else
           inPorts = helper_input_gen_WithInputs(modelName, modelComplexity, hasEnums);
       end
    else
        inPorts = helper_input_gen_WithoutInputs(modelName, modelComplexity, hasTunableParams, hasStructParams, hasStructArrayParams, hasEnums);
    end

end

function name_output_signal(address, signal_index, signal_name)

    p = get_param(address, 'PortHandles');
    l = get_param(p.Inport(signal_index),'Line');
    set_param(l,'Name', signal_name);

end

function name_input_signal(address, signal_index, signal_name)

    p = get_param(address, 'PortHandles');
    l = get_param(p.Outport(signal_index),'Line');
    set_param(l,'Name', signal_name);

end

function log_input_signal(address)
    path = split(address, '/');
    model_name = path{1};
    try
        cm = coder.mapping.api.get(model_name);
    catch
        cm = coder.mapping.utils.create(model_name);
    end
    setDataDefault(cm,"InternalData","StorageClass","MultiInstance");
    p = get_param(address, 'PortHandles');
    l = p.Outport(1);
    addSignal(cm, l);
    setSignal(cm, l, "StorageClass","Model default");
end
