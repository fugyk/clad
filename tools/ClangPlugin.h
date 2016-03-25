//--------------------------------------------------------------------*- C++ -*-
// clad - the C++ Clang-based Automatic Differentiator
// version: $Id$
// author:  Vassil Vassilev <vvasilev-at-cern.ch>
//------------------------------------------------------------------------------

#ifndef CLAD_CLANG_PLUGIN
#define CLAD_CLANG_PLUGIN

#include "clad/Differentiator/Version.h"
#include "clad/Differentiator/DerivativeBuilder.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Version.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/OwningPtr.h"

namespace clang {
  class ASTContext;
  class CallExpr;
  class CompilerInstance;
  class DeclGroupRef;
  class Expr;
//class FunctionDecl;
  class ParmVarDecl;
  class Sema;
}

namespace clad {
  namespace plugin {
    struct DifferentiationOptions {
      DifferentiationOptions()
        : DumpSourceFn(false), DumpSourceFnAST(false), DumpDerivedFn(false),
          DumpDerivedAST(false), DumpDerivedOpenCL(false), GenerateSourceFile(false) { }

      bool DumpSourceFn : 1;
      bool DumpSourceFnAST : 1;
      bool DumpDerivedFn : 1;
      bool DumpDerivedAST : 1;
      bool DumpDerivedOpenCL : 1;
      bool GenerateSourceFile : 1;
    };

    class CladPlugin : public clang::ASTConsumer {
    private:
      clang::CompilerInstance& m_CI;
      DifferentiationOptions m_DO;
      llvm::OwningPtr<DerivativeBuilder> m_DerivativeBuilder;
       bool m_CheckRuntime;
    public:
      CladPlugin(clang::CompilerInstance& CI, DifferentiationOptions& DO);
      ~CladPlugin();

      virtual void Initialize(clang::ASTContext& Context);
      virtual bool HandleTopLevelDecl(clang::DeclGroupRef DGR);
    };

    template<typename ConsumerType>
    class Action : public clang::PluginASTAction {
    private:
      DifferentiationOptions m_DO;
    protected:
      clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance& CI,
                                            llvm::StringRef InFile) {
        return new ConsumerType(CI, m_DO);
      }

      bool ParseArgs(const clang::CompilerInstance &CI,
                     const std::vector<std::string>& args) {
        if (clang::getClangRevision() != clad::getClangCompatRevision()) {
          // TODO: Print nice looking diagnostics through the DiagEngine.
          llvm::errs() << "Clang is not compatible with clad."
                       << " (" << clang::getClangRevision() << " != "
                       << clad::getClangCompatRevision() << " )\n";
          return false;
        }
        for (unsigned i = 0, e = args.size(); i != e; ++i) {
          if (args[i] == "-fdump-source-fn") {
            m_DO.DumpSourceFn = true;
          }
          else if (args[i] == "-fdump-source-fn-ast") {
            m_DO.DumpSourceFnAST = true;
          }
          else if (args[i] == "-fdump-derived-fn") {
            m_DO.DumpDerivedFn = true;
          }
          else if (args[i] == "-fdump-derived-fn-ast") {
            m_DO.DumpDerivedAST = true;
          }
          else if (args[i] == "-fdump-derived-OpenCL") {
            m_DO.DumpDerivedOpenCL= true;
          }
          else if (args[i] == "-fgenerate-source-file") {
            m_DO.GenerateSourceFile = true;
          }
          else if (args[i] == "-help") {
            // Print some help info.
            llvm::errs() <<
              "Option set for the clang-based automatic differentiator - clad:\n\n" <<
              "-fdump-source-fn - Prints out the source code of the function.\n" <<
              "-fdump-source-fn-ast - Prints out the AST of the function.\n" <<
              "-fdump-derived-fn - Prints out the source code of the derivative.\n" <<
              "-fdump-derived-fn-ast - Prints out the AST of the derivative.\n" <<
              "-fdump-derived-OpenCL" <<
              "-fgenerate-source-file - Produces a file containing the derivatives.\n";

            llvm::errs() << "-help - Prints out this screen.\n\n";
          }
          else {
            llvm::errs() << "clad: Error: invalid option "
                         << args[i] << "\n";
            return false; // Tells clang not to create the plugin.
          }
        }
        return true;
      }
    };
  } // end namespace plugin
} // end namespace clad

#endif // CLAD_CLANG_PLUGIN
