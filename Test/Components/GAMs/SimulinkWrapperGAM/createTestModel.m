function [model_compiled, model_name] = createTestModel(varargin)

evalin('base', 'clear matrixConstant vectorConstant structScalar structMixed');
dimensions = ["Scalar", "Vector", "Matrix", "Matrix3D"];
shapes = ["1", "[8 1]", "[6 6]", "[2 3 4]"];
model_compiled = false;

%% settings

% default values
modelComplexity       = 1;
hasAllocFcn           = true;
hasGetmmiFcn          = true;
hasTunableParams      = false;
hasStructParams       = false;
hasStructArrayParams  = false;
hasInputs             = true;
hasOutputs            = true;
hasLoggingSignals     = false;
hasStructSignals      = false;
hasStructArraySignals = false;
hasEnums              = false;
hasSignalNames        = true;
dataOrientation       = 'Column-major';   isRowMajor = 0;
useType               = 0;
debugMode             = false;

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

        case 'hasStructArraySignals'
            hasStructArraySignals = varargin{2};

        case 'hasEnums'
            hasEnums = varargin{2};

        case 'hasSignalNames'
            hasSignalNames = varargin{2};

        case 'dataOrientation'
            dataOrientation = varargin{2};
            if strcmp(dataOrientation, 'Row-major')
                isRowMajor = 1;
            end

        case 'useType'
            useType = varargin{2};

        case 'debugMode'
            debugMode = varargin{2};

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

% warning: the test model (tmd) name is limited to 20 characters
model_name = ['tmd' int2str(modelComplexity)  int2str(hasAllocFcn)       int2str(hasGetmmiFcn) ...
                    int2str(hasTunableParams) int2str(hasStructParams)   int2str(hasStructArrayParams) ...
                    int2str(hasInputs)        int2str(hasOutputs)        int2str(hasStructSignals)     int2str(hasStructArraySignals) ...
                    int2str(isRowMajor)       int2str(hasEnums)          int2str(hasSignalNames) ...
                    int2str(useType)          int2str(hasLoggingSignals) ...
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
                evalin('base', 'matrixConstant3d = ones(2,3,4);');
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
                evalin('base', 'structMixed.mat3d = uint32(ones(2, 3, 4)*2);');
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

if hasEnums
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

% input ports
inPorts = helper_input_gen(model_name, hasInputs, hasStructInputs, modelComplexity, hasTunableParams, hasStructParams, hasStructArrayParams, hasEnums);

for c = 1:modelComplexity
    dim_name = dimensions(c);
    shape = shapes(c);

    signal_indices = string([2*c - 1, 2*c]);
    signal_dim_and_type = strcat(dim_name, ["Double", "Uint32"]);
    in_signal_names  = strcat("In",  signal_indices, "_", signal_dim_and_type);
    out_signal_names = strcat("Out", signal_indices, "_", signal_dim_and_type);
    log_signal_names = strcat("Log", signal_indices, "_", signal_dim_and_type);

    bus_index = join(signal_indices, "");
    bus_prefix = strcat("Out", bus_index, "_");
    bus_name = strcat(bus_prefix, "NonVirtualBus");
    bus_type_name = strcat("STRUCTSIGNAL_T_", bus_index);
    bus_creator_name = strcat(bus_prefix, "BusCreator");

    structarray_index = strcat(bus_index, "0");
    structarray_prefix = strcat("Out", structarray_index, "_");
    structarray_name = strcat(structarray_prefix, dim_name, "StructArray");

    %% ADD OUTPUT PORTS
    if hasOutputs
        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  strcat(model_name, "/", out_signal_names(1)), ...
            'SignalName',     out_signal_names(1), ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'double');
    
        outPorts(end + 1) = add_block('simulink/Sinks/Out1',  strcat(model_name, "/", out_signal_names(2)), ...
            'SignalName',     out_signal_names(2), ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'uint32');

        if hasEnums
            set_param([model_name '/Out2_ScalarUint32'], 'OutDataTypeStr', 'Enum:TestEnum');
        end

        if hasStructSignals
            outPorts(end + 1) = add_block('simulink/Sinks/Out1',  strcat(model_name, "/", bus_name), ...
                'SignalName',     strcat(bus_prefix, "NonVirtualBus"), ...
                'IconDisplay',    'Signal name', ...
                'OutDataTypeStr', 'Inherit: auto');

            % bus of Buses
            if c == 4
                outPorts(end + 1) = add_block('simulink/Sinks/Out1',  [model_name '/Out3456_NonVirtualBus'], ...
                    'SignalName',     'Out3456_NonVirtualBus', ...
                    'IconDisplay',    'Signal name', ...
                    'OutDataTypeStr', 'Inherit: auto');
            end
        end

        if hasStructArraySignals
            outPorts(end + 1) = add_block('simulink/Sinks/Out1', strcat(model_name, "/", structarray_name), ...
                'SignalName',     structarray_name, ...
                'IconDisplay',    'Signal name', ...
                'OutDataTypeStr', 'Inherit: auto', ...
                'PortDimensions', '[1 3]');
        end

    else % hasOutputs == false
        add_block('simulink/Sinks/Terminator', strcat(model_name, "/", out_signal_names(1)));
        add_block('simulink/Sinks/Terminator', strcat(model_name, "/", out_signal_names(2)));

        if hasStructSignals
            add_block('simulink/Sinks/Terminator',  strcat(model_name, "/", bus_name));

            % bus of Buses
            if c == 4
                add_block('simulink/Sinks/Terminator',  [model_name '/Out3456_NonVirtualBus']);
            end
        end

        if hasStructArraySignals
            add_block('simulink/Sinks/Out1', strcat(model_name, "/", structarray_name));
        end
    end

    %% DEFINE BUSES
    if hasStructSignals || hasStructArraySignals
        evalin('base', 'clear busElems');

        evalin('base', strcat( ...
            'busElems(1) = Simulink.BusElement;', ...
            'busElems(1).Name = "', signal_dim_and_type(1), '";', ...
            'busElems(1).Dimensions = ', shape, ';', ...
            'busElems(1).DimensionsMode = "Fixed";', ...
            'busElems(1).DataType = "double";', ...
            'busElems(1).SampleTime = -1;', ...
            'busElems(1).Complexity = "real";' ...
            ));

        evalin('base', strcat( ...
            'busElems(2) = Simulink.BusElement;', ...
            'busElems(2).Name = "', signal_dim_and_type(2), '";', ...
            'busElems(2).Dimensions = ', shape, ';', ...
            'busElems(2).DimensionsMode = "Fixed";', ...
            'busElems(2).DataType = "uint32";', ...
            'busElems(2).SampleTime = -1;', ...
            'busElems(2).Complexity = "real";' ...
            ));

        evalin('base', [ ...
            char(bus_type_name) ' = Simulink.Bus;' ...
            char(bus_type_name) '.Elements = busElems;' ...
            ]);

        add_block('simulink/Signal Routing/Bus Creator', strcat(model_name, "/", bus_creator_name), ...
            'Inputs',         '2', ...
            'NonVirtualBus',  'on', ...
            'OutDataTypeStr', strcat('Bus: ', bus_type_name));

        % bus of buses
        if c == 4
            evalin('base', 'clear busElems;');

            evalin('base', strcat( ...
                'busElems(1) = Simulink.BusElement;', ...
                'busElems(1).Name = "Vector_Structured";', ...
                'busElems(1).Dimensions = 1;', ...
                'busElems(1).DimensionsMode = "Fixed";', ...
                'busElems(1).DataType = "Bus: STRUCTSIGNAL_T_34";', ...
                'busElems(1).SampleTime = -1;', ...
                'busElems(1).Complexity = "real";' ...
                ));
    
            evalin('base', strcat( ...
                'busElems(2) = Simulink.BusElement;', ...
                'busElems(2).Name = "Matrix_Structured";', ...
                'busElems(2).Dimensions = 1;', ...
                'busElems(2).DimensionsMode = "Fixed";', ...
                'busElems(2).DataType = "Bus: STRUCTSIGNAL_T_56";', ...
                'busElems(2).SampleTime = -1;', ...
                'busElems(2).Complexity = "real";' ...
                ));
    
            evalin('base', strcat( ...
                'STRUCTSIGNAL_T_3456 = Simulink.Bus;', ...
                'STRUCTSIGNAL_T_3456.Elements = busElems;' ...
                ));
    
            add_block('simulink/Signal Routing/Bus Creator', [model_name '/Out3456_BusCreator'], ...
                'Inputs',         '2', ...
                'NonVirtualBus',  'on', ...
                'OutDataTypeStr', 'Bus: STRUCTSIGNAL_T_3456');
        end
    end

    %% ADD BLOCKS
    gain_block_names = strcat("Gain", signal_indices, "_", signal_dim_and_type);

    add_block('simulink/Math Operations/Gain', strcat(model_name, "/", gain_block_names(1)), ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'double');
    add_block('simulink/Math Operations/Gain', strcat(model_name, "/", gain_block_names(2)), ...
        'Gain',           '1', ...
        'OutDataTypeStr', 'uint32');

    if hasEnums
        % gain block does not support enums
        delete_block([model_name '/Gain2_ScalarUint32']);
        add_block('simulink/Signal Attributes/Signal Specification', [model_name '/Gain2_ScalarUint32' ], ...
                'OutDataTypeStr', 'Enum:TestEnum');
    end

    if hasStructParams && c == 2
        set_param(strcat(model_name, "/", gain_block_names(1)), 'Gain', 'structMixed.one');
    end

    if hasLoggingSignals
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', strcat(model_name, "/", log_signal_names(1)), ...
            'Gain',           '1', ...
            'OutDataTypeStr', 'double');
        logBlocks(end + 1) = add_block('simulink/Math Operations/Gain', strcat(model_name, "/", log_signal_names(2)), ...
            'Gain',           '1', ...
            'OutDataTypeStr', 'uint32');

        if hasEnums
            set_param([model_name, '/Log2_ScalarUint32'], 'OutDataTypeStr', 'Enum:TestEnum');
        end

        if hasStructSignals
            % empty subsystem to create structured signals for logging
            subsystem_name = strcat("/Log", bus_index, "_NonVirtualBus");
            logBlocks(end + 1) = add_block('simulink/Ports & Subsystems/Atomic Subsystem', strcat(model_name, subsystem_name));

            % bus of buses
            if c == 4
                subsystem_name = strcat("/Log3456_NonVirtualBus");
                logBlocks(end + 1) = add_block('simulink/Ports & Subsystems/Atomic Subsystem', strcat(model_name, subsystem_name));
            end
        end

        if hasStructArraySignals
            subsystem_name = strcat("/Log", structarray_index, "_StructArray");
            logBlocks(end + 1) = add_block('simulink/Ports & Subsystems/Atomic Subsystem', strcat(model_name, subsystem_name));
        end
    end

    if hasStructArraySignals
        % generate structarray fields
        struct_array = evalin('base', ['Simulink.Bus.createMATLABStruct("' char(bus_type_name) '")']);
        structarray_shape = [1 2];
        structarray_num_elems = prod(structarray_shape);
        struct_array = repmat(struct_array, structarray_shape);
        for sa_idx = 1:structarray_num_elems
            for field = string(fieldnames(struct_array(sa_idx)))'
                field_num_elems = numel(struct_array(sa_idx).(field));
                field_type_name = class(struct_array(sa_idx).(field));
                field_shape     = size(struct_array(sa_idx).(field));
                
                field_value = reshape(1:field_num_elems, field_shape);
                struct_array(sa_idx).(field) = cast(field_value, field_type_name);
            end
        end
        var_name = strcat(bus_prefix, "ConstStructArray");
        assignin('base', var_name, struct_array);

        concat_block_name = strcat(structarray_prefix, "Concatenate");
        const_block_name = strcat(structarray_prefix, "Constant");

        add_block('simulink/Commonly Used Blocks/Vector Concatenate', strcat(model_name, "/", concat_block_name));

        add_block('simulink/Sources/Constant', strcat(model_name, "/", const_block_name), ...
            'Value',          var_name, ...
            'OutDataTypeStr', strcat('Bus: ', bus_type_name));
    end

    %% ADD SIGNALS
    if hasStructSignals
        if c == 4
            add_line(model_name, 'Out34_BusCreator/1',   'Out3456_BusCreator/1');
            add_line(model_name, 'Out56_BusCreator/1',   'Out3456_BusCreator/2');
        end
    end
    
    if hasStructArraySignals
        add_line(model_name, strcat(const_block_name,  "/1"), strcat(concat_block_name, "/1"));
        add_line(model_name, strcat(bus_creator_name,  "/1"), strcat(concat_block_name, "/2"));
    end

    if hasLoggingSignals == false
        add_line(model_name, strcat(in_signal_names(1), "/1"), strcat(gain_block_names(1), "/1"));
        add_line(model_name, strcat(in_signal_names(2), "/1"), strcat(gain_block_names(2), "/1"));

        add_line(model_name, strcat(gain_block_names(1), "/1"), strcat(out_signal_names(1), "/1"));
        add_line(model_name, strcat(gain_block_names(2), "/1"), strcat(out_signal_names(2), "/1"));

        if hasStructSignals
            add_line(model_name, strcat(gain_block_names(1), "/1"), strcat(bus_creator_name, "/1"));
            add_line(model_name, strcat(gain_block_names(2), "/1"), strcat(bus_creator_name, "/2"));
            add_line(model_name, strcat(bus_creator_name,    "/1"), strcat(bus_name,         "/1"));
    
            % bus of buses
            if c == 4
                add_line(model_name, 'Out3456_BusCreator/1', 'Out3456_NonVirtualBus/1');
            end
        end
    
        if hasStructArraySignals
            add_line(model_name, strcat(concat_block_name, "/1"), strcat(structarray_name,  "/1"));
        end

    else % hasLoggingSignals == true
        add_line(model_name, strcat(in_signal_names(1), "/1"), strcat(gain_block_names(1), "/1"));
        add_line(model_name, strcat(in_signal_names(2), "/1"), strcat(gain_block_names(2), "/1"));

        add_line(model_name, strcat(gain_block_names(1), "/1"), strcat(log_signal_names(1), "/1"));
        add_line(model_name, strcat(gain_block_names(2), "/1"), strcat(log_signal_names(2), "/1"));

        add_line(model_name, strcat(log_signal_names(1), "/1"), strcat(out_signal_names(1), "/1"));
        add_line(model_name, strcat(log_signal_names(2), "/1"), strcat(out_signal_names(2), "/1"));

        if hasStructSignals
            add_line(model_name, strcat(log_signal_names(1), "/1"), strcat(bus_creator_name, "/1"));
            add_line(model_name, strcat(log_signal_names(2), "/1"), strcat(bus_creator_name, "/2"));
            add_line(model_name, strcat(bus_creator_name,    "/1"), strcat("Log", bus_index, "_NonVirtualBus/1"));
            add_line(model_name, strcat("Log", bus_index, "_NonVirtualBus/1"), strcat(bus_name,         "/1"));
    
            % bus of buses
            if c == 4
                add_line(model_name, 'Out3456_BusCreator/1', 'Log3456_NonVirtualBus/1');
                add_line(model_name, 'Log3456_NonVirtualBus/1', 'Out3456_NonVirtualBus/1');
            end
        end
    
        if hasStructArraySignals
            add_line(model_name, strcat(concat_block_name, "/1"), strcat("Log", structarray_index, "_StructArray/1"));
            add_line(model_name, strcat("Log", structarray_index, "_StructArray/1"), strcat(structarray_name,  "/1"));
        end
    end

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
set_param([model_name '/Gain1_ScalarDouble'], ...
    'Gain',           gain1Param, ...
    'OutDataTypeStr', 'double');

if ~hasEnums
    set_param([model_name '/Gain2_ScalarUint32'], 'Gain',           gain2Param);
    set_param([model_name '/Gain2_ScalarUint32'], 'OutDataTypeStr',   'uint32');
    set_param([model_name '/Gain2_ScalarUint32'], 'ParamDataTypeStr', 'uint32');
end

% change types if requested
if useType == 1

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'uint8');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'uint16');
        set_param([model_name '/Gain5_MatrixDouble'], 'OutDataTypeStr', 'uint8');
        set_param([model_name '/Gain6_MatrixUint32'], 'OutDataTypeStr', 'uint16');
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
        set_param([model_name '/In7_Matrix3DDouble'],   'OutDataTypeStr', 'uint32');
        set_param([model_name '/Gain7_Matrix3DDouble'], 'OutDataTypeStr', 'uint32');
        %set_param([model_name '/In8_Matrix3DUint32'], 'OutDataTypeStr', 'uint64'); % uint64 not yet supported by coder
        if hasOutputs == true
            set_param([model_name '/Out7_Matrix3DDouble'], 'OutDataTypeStr', 'uint32');
            %set_param([model_name '/Out8_Matrix3DUint32'], 'OutDataTypeStr', 'uint64'); % uint64 not yet supported by coder
        end
    end

end

if useType == 2

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'int8');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'int16');
        set_param([model_name '/Gain5_MatrixDouble'], 'OutDataTypeStr', 'int8');
        set_param([model_name '/Gain6_MatrixUint32'], 'OutDataTypeStr', 'int16');
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
        set_param([model_name '/In7_Matrix3DDouble'],   'OutDataTypeStr', 'int32');
        set_param([model_name '/Gain7_Matrix3DDouble'], 'OutDataTypeStr', 'int32');
        %set_param([model_name '/In8_Matrix3DUint32'], 'OutDataTypeStr', 'int64'); % int64 not yet supported by coder
        if hasOutputs == true
            set_param([model_name '/Out7_Matrix3DDouble'], 'OutDataTypeStr', 'int32');
            %set_param([model_name '/Out8_Matrix3DUint32'], 'OutDataTypeStr', 'int64'); % int64 not yet supported by coder
        end
    end

end

if useType == 4

    if modelComplexity >= 3
        set_param([model_name '/In5_MatrixDouble'],   'OutDataTypeStr', 'int32');
        set_param([model_name '/In6_MatrixUint32'],   'OutDataTypeStr', 'single');
        set_param([model_name '/Gain5_MatrixDouble'], 'OutDataTypeStr', 'int32');
        set_param([model_name '/Gain6_MatrixUint32'], 'OutDataTypeStr', 'single');
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
        set_param([model_name '/In7_Matrix3DDouble'], 'OutDataTypeStr', 'int32');
        set_param([model_name '/In8_Matrix3DUint32'], 'OutDataTypeStr', 'single');
        set_param([model_name '/Gain7_MatrixDouble'], 'OutDataTypeStr', 'int32');
        set_param([model_name '/Gain8_MatrixUint32'], 'OutDataTypeStr', 'single');
        if hasOutputs == true
            set_param([model_name '/Out7_Matrix3DDouble'], 'OutDataTypeStr', 'int32');
            set_param([model_name '/Out8_Matrix3DUint32'], 'OutDataTypeStr', 'single');
        end
    end

end

%% signal naming

% name the output signals
for port = outPorts
    signal_name = get_param(port, 'Name');
    p           = get_param(port, 'PortHandles');
    l           = get_param(p.Inport(1), 'Line');
    if hasSignalNames, set_param(l, 'Name', signal_name); end
end

% name the input signals
for port = inPorts
    signal_name = get_param(port, 'Name');
    p           = get_param(port, 'PortHandles');
    l           = get_param(p.Outport(1), 'Line');
    if hasSignalNames, set_param(l, 'Name', signal_name); end
end

% name the logging signals
if hasLoggingSignals
    for block = logBlocks
        signal_name = get_param(block, 'Name');
        p           = get_param(block, 'PortHandles');
        l           = get_param(p.Inport(1), 'Line');
        if hasSignalNames, set_param(l, 'Name', signal_name); end
    end
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
%         p = get_param(block, 'PortHandles');
%         l = p.Inport(1);
        % for some reason the commented lines above result in a signal with suffix `:1` to be added to the 
        % logging signals, and such signal then is not generated in the code. We use a workaround:
        conn = get_param(block, 'PortConnectivity');
        src_block = conn(1).SrcBlock;
        p = get_param(src_block, 'PortHandles');
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

if ~debugMode
    try
        rtwbuild(model_name)
        model_compiled = true;
    catch ME
        model_compiled = false;
        warning('on', ME.identifier);
        warning(ME.identifier, '%s', ME.message)
    end
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

if ~debugMode
    delete(sprintf('%s.slx',model_name));
end
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
        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In7_Matrix3DDouble' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'double', ...
            'PortDimensions',  '[2 3 4]');

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In8_Matrix3DUint32' ], ...
            'IconDisplay',    'Signal name', ...
            'OutdataTypeStr', 'uint32', ...
            'PortDimensions',  '[2 3 4]');
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
            set_param([model_name '/In3_VectorDouble'],  'Value',          '7:-1:0');
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
            set_param([model_name '/In5_MatrixDouble'], 'Value',          'reshape([1:18 1:18], 6, 6)');
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
        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In7_Matrix3DDouble'], ...
            'OutDataTypeStr', 'double');
        if hasTunableParams == true
            set_param([model_name '/In7_Matrix3DDouble'],  'Value',          'matrixConstant3d');
        else
            set_param([model_name '/In7_Matrix3DDouble'], 'Value',          'reshape(1:24, [2 3 4])');
        end

        tempInPorts(end + 1) = add_block('simulink/Sources/Constant', [model_name '/In8_Matrix3DUint32'], ...
            'OutDataTypeStr', 'uint32');
        if hasStructParams == true
            set_param([model_name '/In8_Matrix3DUint32'], 'Value',          'structMixed.mat3d');
        else
            set_param([model_name '/In8_Matrix3DUint32'],  'Value',          'reshape(uint32(1:24), [2 3 4])');
        end
    end

end

function tempInPorts = helper_input_gen_WithStructInputs(model_name, modelComplexity)

    tempInPorts = [];

    tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In1_Structured'], ...
        'IconDisplay',    'Signal name', ...
        'OutDataTypeStr', 'Bus: STRUCTSIGNAL_T_12', ...
        'BusOutputAsStruct',  'on');

    add_block('simulink/Math Operations/Gain', [model_name '/In1_ScalarDouble'], ...
              'OutDataTypeStr', 'double');
    add_block('simulink/Math Operations/Gain', [model_name '/In2_ScalarUint32'], ...
              'OutDataTypeStr', 'uint32');

    add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector'], ...
        'OutputSignals', 'ScalarDouble,ScalarUint32');

    add_line(model_name, 'InputSelector/1', 'In1_ScalarDouble/1');
    add_line(model_name, 'InputSelector/2', 'In2_ScalarUint32/1');

    add_line(model_name, 'In1_Structured/1', 'InputSelector/1');
    name_input_signal([model_name '/In1_Structured'], 1, 'In1_Structured');

    if modelComplexity > 1

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In2_Structured'], ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Bus: STRUCTSIGNAL_T_34', ...
            'BusOutputAsStruct',  'on');

        add_block('simulink/Math Operations/Gain', [model_name '/In3_VectorDouble'], ...
            'OutDataTypeStr', 'double');
        add_block('simulink/Math Operations/Gain', [model_name '/In4_VectorUint32'], ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector2'], ...
            'OutputSignals', 'VectorDouble,VectorUint32');

        add_line(model_name, 'InputSelector2/1', 'In3_VectorDouble/1');
        add_line(model_name, 'InputSelector2/2', 'In4_VectorUint32/1');

        add_line(model_name, 'In2_Structured/1', 'InputSelector2/1');
        name_input_signal([model_name '/In2_Structured'], 1, 'In2_Structured');
    end

    if modelComplexity > 2

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In3_Structured'], ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Bus: STRUCTSIGNAL_T_56', ...
            'BusOutputAsStruct',  'on');

        add_block('simulink/Math Operations/Gain', [model_name '/In5_MatrixDouble'], ...
            'OutDataTypeStr', 'double');
        add_block('simulink/Math Operations/Gain', [model_name '/In6_MatrixUint32'], ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector3'], ...
            'OutputSignals', 'MatrixDouble,MatrixUint32');

        add_line(model_name, 'InputSelector3/1', 'In5_MatrixDouble/1');
        add_line(model_name, 'InputSelector3/2', 'In6_MatrixUint32/1');

        add_line(model_name, 'In3_Structured/1', 'InputSelector3/1');
        name_input_signal([model_name '/In3_Structured'], 1, 'In3_Structured');
    end

    if modelComplexity > 3

        tempInPorts(end + 1) = add_block('simulink/Sources/In1', [model_name '/In4_Structured'], ...
            'IconDisplay',    'Signal name', ...
            'OutDataTypeStr', 'Bus: STRUCTSIGNAL_T_78', ...
            'BusOutputAsStruct',  'on');

        add_block('simulink/Math Operations/Gain', [model_name '/In7_Matrix3DDouble'], ...
            'OutDataTypeStr', 'double');
        add_block('simulink/Math Operations/Gain', [model_name '/In8_Matrix3DUint32'], ...
            'OutDataTypeStr', 'uint32');

        add_block('simulink/Signal Routing/Bus Selector', [model_name '/InputSelector4'], ...
            'OutputSignals', 'Matrix3DDouble,Matrix3DUint32');

        add_line(model_name, 'InputSelector4/1', 'In7_Matrix3DDouble/1');
        add_line(model_name, 'InputSelector4/2', 'In8_Matrix3DUint32/1');

        add_line(model_name, 'In4_Structured/1', 'InputSelector4/1');

        name_input_signal([model_name '/In4_Structured'], 1, 'In4_Structured');
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
