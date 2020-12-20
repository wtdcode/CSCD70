#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"

namespace {
    class LocalOptsPass : public llvm::FunctionPass{
        public:
            static char ID;

            LocalOptsPass() : llvm::FunctionPass(ID){}

            virtual bool runOnFunction(llvm::Function &F){
                llvm::outs() << "Hello world!\n";
                return true;
            }
    };
}

char LocalOptsPass::ID = 0;
static llvm::RegisterPass<LocalOptsPass> X("lopt", "LocalOpts pass", false, false);