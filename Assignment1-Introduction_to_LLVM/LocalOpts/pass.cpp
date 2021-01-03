#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

namespace {
    class LocalOptsPass : public llvm::FunctionPass{
        public:
            static char ID;

            LocalOptsPass() : llvm::FunctionPass(ID){}

            virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override {
                AU.setPreservesAll();
            }

            std::size_t AlgebraicIdentity(llvm::Function &F){
                std::size_t count = 0;
                std::vector<llvm::Instruction *> to_remove;
                for(auto bb = F.begin(); bb != F.end(); bb++){
                    for(auto it = bb->begin(); it != bb->end(); it++){
                        auto opcode = it->getOpcode();
                        switch (opcode){
                            case llvm::Instruction::Add:
                            case llvm::Instruction::FAdd:
                                if(auto op = llvm::dyn_cast<llvm::ConstantInt>(it->getOperand(0))){
                                    auto it2 = it->getOperand(1);
                                    if(llvm::dyn_cast<llvm::Instruction>(it2) && op->getZExtValue() == 0){
                                        it->replaceAllUsesWith(it2);
                                        to_remove.push_back(&(*it));
                                    }
                                }
                            // Fallthrough!
                            case llvm::Instruction::Sub:
                            case llvm::Instruction::FSub:
                                if(auto op = llvm::dyn_cast<llvm::ConstantInt>(it->getOperand(1))){
                                    auto it2 = it->getOperand(0);
                                    if(llvm::dyn_cast<llvm::Instruction>(it2) && op->getZExtValue() == 0){
                                        it->replaceAllUsesWith(it2);
                                        to_remove.push_back(&(*it));
                                    }
                                }
                                break;
                            case llvm::Instruction::FMul:
                            case llvm::Instruction::Mul:
                                if(auto op = llvm::dyn_cast<llvm::ConstantInt>(it->getOperand(0))){
                                    auto it2 = it->getOperand(1);
                                    if(llvm::dyn_cast<llvm::Instruction>(it2) && op->getZExtValue() == 1){
                                        it->replaceAllUsesWith(it2);
                                        to_remove.push_back(&(*it));
                                    }
                                }
                            case llvm::Instruction::SDiv:
                            case llvm::Instruction::UDiv:
                            case llvm::Instruction::FDiv:
                                if(auto op = llvm::dyn_cast<llvm::ConstantInt>(it->getOperand(1))){
                                    auto it2 = it->getOperand(0);
                                    if(llvm::dyn_cast<llvm::Instruction>(it2) && op->getZExtValue() == 1){
                                        it->replaceAllUsesWith(it2);
                                        to_remove.push_back(&(*it));
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
                for(auto it = to_remove.begin(); it != to_remove.end(); it++){
                    if((*it)->isSafeToRemove()){
                        (*it)->eraseFromParent();
                        count += 1;
                    }
                    else{
                        llvm::outs() << "Warning: " << *(*it) << "not safe to be removed!\n";
                    }
                }
                return count;
            }

            virtual bool runOnFunction(llvm::Function &F){
                llvm::outs() << "Start to perform LocalOpts for function " << F.getName() << "!\n";
                auto algebraic_identity_count = this->AlgebraicIdentity(F);
                llvm::outs() << "Algebraic Identity Count: " << algebraic_identity_count << "\n";
                return true;
            }
    };
}

char LocalOptsPass::ID = 0;
static llvm::RegisterPass<LocalOptsPass> X("lopt", "LocalOpts pass", false, false);