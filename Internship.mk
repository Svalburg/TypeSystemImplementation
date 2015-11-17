##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Internship
ConfigurationName      :=Debug
WorkspacePath          := "C:\Users\Sven\Documents\GitHub\TypeSystemImplementation"
ProjectPath            := "C:\Users\Sven\Documents\GitHub\TypeSystemImplementation"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Sven
Date                   :=17/11/2015
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=bin/Debug/Internship
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Internship.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g -Wall -fexceptions  $(Preprocessors)
CFLAGS   :=  -g -Wall -fexceptions  $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Rule.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleAssign.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleBinOp.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleCallCmpF.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleCallF.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleConst.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleExprConcat.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleFuncDef.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_RuleIf.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleInput.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleRepeat.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleSkip.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleStmtConcat.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_RuleVar.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_StateTuple.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ComponentFunction.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_TimeDependentEC.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/src_Rule.cpp$(ObjectSuffix): src/Rule.cpp $(IntermediateDirectory)/src_Rule.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/Rule.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Rule.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Rule.cpp$(DependSuffix): src/Rule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Rule.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Rule.cpp$(DependSuffix) -MM "src/Rule.cpp"

$(IntermediateDirectory)/src_Rule.cpp$(PreprocessSuffix): src/Rule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Rule.cpp$(PreprocessSuffix) "src/Rule.cpp"

$(IntermediateDirectory)/src_RuleAssign.cpp$(ObjectSuffix): src/RuleAssign.cpp $(IntermediateDirectory)/src_RuleAssign.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleAssign.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleAssign.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleAssign.cpp$(DependSuffix): src/RuleAssign.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleAssign.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleAssign.cpp$(DependSuffix) -MM "src/RuleAssign.cpp"

$(IntermediateDirectory)/src_RuleAssign.cpp$(PreprocessSuffix): src/RuleAssign.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleAssign.cpp$(PreprocessSuffix) "src/RuleAssign.cpp"

$(IntermediateDirectory)/src_RuleBinOp.cpp$(ObjectSuffix): src/RuleBinOp.cpp $(IntermediateDirectory)/src_RuleBinOp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleBinOp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleBinOp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleBinOp.cpp$(DependSuffix): src/RuleBinOp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleBinOp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleBinOp.cpp$(DependSuffix) -MM "src/RuleBinOp.cpp"

$(IntermediateDirectory)/src_RuleBinOp.cpp$(PreprocessSuffix): src/RuleBinOp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleBinOp.cpp$(PreprocessSuffix) "src/RuleBinOp.cpp"

$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(ObjectSuffix): src/RuleCallCmpF.cpp $(IntermediateDirectory)/src_RuleCallCmpF.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleCallCmpF.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(DependSuffix): src/RuleCallCmpF.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(DependSuffix) -MM "src/RuleCallCmpF.cpp"

$(IntermediateDirectory)/src_RuleCallCmpF.cpp$(PreprocessSuffix): src/RuleCallCmpF.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleCallCmpF.cpp$(PreprocessSuffix) "src/RuleCallCmpF.cpp"

$(IntermediateDirectory)/src_RuleCallF.cpp$(ObjectSuffix): src/RuleCallF.cpp $(IntermediateDirectory)/src_RuleCallF.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleCallF.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleCallF.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleCallF.cpp$(DependSuffix): src/RuleCallF.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleCallF.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleCallF.cpp$(DependSuffix) -MM "src/RuleCallF.cpp"

$(IntermediateDirectory)/src_RuleCallF.cpp$(PreprocessSuffix): src/RuleCallF.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleCallF.cpp$(PreprocessSuffix) "src/RuleCallF.cpp"

$(IntermediateDirectory)/src_RuleConst.cpp$(ObjectSuffix): src/RuleConst.cpp $(IntermediateDirectory)/src_RuleConst.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleConst.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleConst.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleConst.cpp$(DependSuffix): src/RuleConst.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleConst.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleConst.cpp$(DependSuffix) -MM "src/RuleConst.cpp"

$(IntermediateDirectory)/src_RuleConst.cpp$(PreprocessSuffix): src/RuleConst.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleConst.cpp$(PreprocessSuffix) "src/RuleConst.cpp"

$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(ObjectSuffix): src/RuleExprAsStmt.cpp $(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleExprAsStmt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(DependSuffix): src/RuleExprAsStmt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(DependSuffix) -MM "src/RuleExprAsStmt.cpp"

$(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(PreprocessSuffix): src/RuleExprAsStmt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleExprAsStmt.cpp$(PreprocessSuffix) "src/RuleExprAsStmt.cpp"

$(IntermediateDirectory)/src_RuleExprConcat.cpp$(ObjectSuffix): src/RuleExprConcat.cpp $(IntermediateDirectory)/src_RuleExprConcat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleExprConcat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleExprConcat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleExprConcat.cpp$(DependSuffix): src/RuleExprConcat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleExprConcat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleExprConcat.cpp$(DependSuffix) -MM "src/RuleExprConcat.cpp"

$(IntermediateDirectory)/src_RuleExprConcat.cpp$(PreprocessSuffix): src/RuleExprConcat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleExprConcat.cpp$(PreprocessSuffix) "src/RuleExprConcat.cpp"

$(IntermediateDirectory)/src_RuleFuncDef.cpp$(ObjectSuffix): src/RuleFuncDef.cpp $(IntermediateDirectory)/src_RuleFuncDef.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleFuncDef.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleFuncDef.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleFuncDef.cpp$(DependSuffix): src/RuleFuncDef.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleFuncDef.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleFuncDef.cpp$(DependSuffix) -MM "src/RuleFuncDef.cpp"

$(IntermediateDirectory)/src_RuleFuncDef.cpp$(PreprocessSuffix): src/RuleFuncDef.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleFuncDef.cpp$(PreprocessSuffix) "src/RuleFuncDef.cpp"

$(IntermediateDirectory)/src_RuleIf.cpp$(ObjectSuffix): src/RuleIf.cpp $(IntermediateDirectory)/src_RuleIf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleIf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleIf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleIf.cpp$(DependSuffix): src/RuleIf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleIf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleIf.cpp$(DependSuffix) -MM "src/RuleIf.cpp"

$(IntermediateDirectory)/src_RuleIf.cpp$(PreprocessSuffix): src/RuleIf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleIf.cpp$(PreprocessSuffix) "src/RuleIf.cpp"

$(IntermediateDirectory)/src_RuleInput.cpp$(ObjectSuffix): src/RuleInput.cpp $(IntermediateDirectory)/src_RuleInput.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleInput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleInput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleInput.cpp$(DependSuffix): src/RuleInput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleInput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleInput.cpp$(DependSuffix) -MM "src/RuleInput.cpp"

$(IntermediateDirectory)/src_RuleInput.cpp$(PreprocessSuffix): src/RuleInput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleInput.cpp$(PreprocessSuffix) "src/RuleInput.cpp"

$(IntermediateDirectory)/src_RuleRepeat.cpp$(ObjectSuffix): src/RuleRepeat.cpp $(IntermediateDirectory)/src_RuleRepeat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleRepeat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleRepeat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleRepeat.cpp$(DependSuffix): src/RuleRepeat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleRepeat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleRepeat.cpp$(DependSuffix) -MM "src/RuleRepeat.cpp"

$(IntermediateDirectory)/src_RuleRepeat.cpp$(PreprocessSuffix): src/RuleRepeat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleRepeat.cpp$(PreprocessSuffix) "src/RuleRepeat.cpp"

$(IntermediateDirectory)/src_RuleSkip.cpp$(ObjectSuffix): src/RuleSkip.cpp $(IntermediateDirectory)/src_RuleSkip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleSkip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleSkip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleSkip.cpp$(DependSuffix): src/RuleSkip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleSkip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleSkip.cpp$(DependSuffix) -MM "src/RuleSkip.cpp"

$(IntermediateDirectory)/src_RuleSkip.cpp$(PreprocessSuffix): src/RuleSkip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleSkip.cpp$(PreprocessSuffix) "src/RuleSkip.cpp"

$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(ObjectSuffix): src/RuleStmtConcat.cpp $(IntermediateDirectory)/src_RuleStmtConcat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleStmtConcat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(DependSuffix): src/RuleStmtConcat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(DependSuffix) -MM "src/RuleStmtConcat.cpp"

$(IntermediateDirectory)/src_RuleStmtConcat.cpp$(PreprocessSuffix): src/RuleStmtConcat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleStmtConcat.cpp$(PreprocessSuffix) "src/RuleStmtConcat.cpp"

$(IntermediateDirectory)/src_RuleVar.cpp$(ObjectSuffix): src/RuleVar.cpp $(IntermediateDirectory)/src_RuleVar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/RuleVar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_RuleVar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_RuleVar.cpp$(DependSuffix): src/RuleVar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_RuleVar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_RuleVar.cpp$(DependSuffix) -MM "src/RuleVar.cpp"

$(IntermediateDirectory)/src_RuleVar.cpp$(PreprocessSuffix): src/RuleVar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_RuleVar.cpp$(PreprocessSuffix) "src/RuleVar.cpp"

$(IntermediateDirectory)/src_StateTuple.cpp$(ObjectSuffix): src/StateTuple.cpp $(IntermediateDirectory)/src_StateTuple.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/StateTuple.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_StateTuple.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_StateTuple.cpp$(DependSuffix): src/StateTuple.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_StateTuple.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_StateTuple.cpp$(DependSuffix) -MM "src/StateTuple.cpp"

$(IntermediateDirectory)/src_StateTuple.cpp$(PreprocessSuffix): src/StateTuple.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_StateTuple.cpp$(PreprocessSuffix) "src/StateTuple.cpp"

$(IntermediateDirectory)/src_ComponentFunction.cpp$(ObjectSuffix): src/ComponentFunction.cpp $(IntermediateDirectory)/src_ComponentFunction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/ComponentFunction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ComponentFunction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ComponentFunction.cpp$(DependSuffix): src/ComponentFunction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ComponentFunction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ComponentFunction.cpp$(DependSuffix) -MM "src/ComponentFunction.cpp"

$(IntermediateDirectory)/src_ComponentFunction.cpp$(PreprocessSuffix): src/ComponentFunction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ComponentFunction.cpp$(PreprocessSuffix) "src/ComponentFunction.cpp"

$(IntermediateDirectory)/src_TimeDependentEC.cpp$(ObjectSuffix): src/TimeDependentEC.cpp $(IntermediateDirectory)/src_TimeDependentEC.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Sven/Documents/GitHub/TypeSystemImplementation/src/TimeDependentEC.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_TimeDependentEC.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_TimeDependentEC.cpp$(DependSuffix): src/TimeDependentEC.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_TimeDependentEC.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_TimeDependentEC.cpp$(DependSuffix) -MM "src/TimeDependentEC.cpp"

$(IntermediateDirectory)/src_TimeDependentEC.cpp$(PreprocessSuffix): src/TimeDependentEC.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_TimeDependentEC.cpp$(PreprocessSuffix) "src/TimeDependentEC.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


