#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;


namespace {

class FunctionInfo final : public ModulePass
{
public:
	static char ID;

	FunctionInfo() : ModulePass(ID) {}
	virtual ~FunctionInfo() override {}

  	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const override
	{
		AU.setPreservesAll();
	}
	/*
	 * The runOnModule method performs the interesting work of the pass. 
	 * It should return true if the module was modified by the transformation 
	 * and false otherwise.
	 */
	virtual bool runOnModule(Module & M) override
	{
		outs() << "CSCD70 Functions Information Pass" << "\n";

		// @TODO Please complete this function.
		
		for(auto it = M.begin(); it != M.end(); it++){
			std::size_t instruction_count = 0;
			std::size_t arguments_count = 0;
			std::size_t bblocks_count = 0;
			std::size_t calls_count = 0;
			outs() << "Name: " << it->getName() << "\n" \
				   << "ID: " << it->getGlobalIdentifier() << "\n";
			instruction_count = it->getInstructionCount();
			for(auto it2 = it->arg_begin(); it2 != it->arg_end(); it2 ++)
				arguments_count += 1;
			auto& bblist = it->getBasicBlockList();
			bblocks_count = bblist.size();
			for(auto instr = it->begin(); instr != it->end(); instr++)
				if(auto callistr = dyn_cast<CallInst>(instr))
					calls_count +=1;
			outs() << "Args: " << arguments_count << "\n" \
			   << "Calls: " << calls_count << "\n" \
			   << "Blocks: " << bblocks_count << "\n" \
			   << "Insts: "  << instruction_count << "\n";
		}
		

		return false;
	}
};

char FunctionInfo::ID = 0;
RegisterPass < FunctionInfo > X (
	"function-info",
	"CSCD70: Functions Information");

}  // namespace anonymous
