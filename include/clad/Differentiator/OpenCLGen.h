//--------------------------------------------------------------------*- C++ -*-
// clad - the C++ Clang-based Automatic Differentiator
// version: $Id$
// author:  Vassil Vassilev <vvasilev-at-cern.ch>
//------------------------------------------------------------------------------
//
// File originates from the Scout project (http://scout.zih.tu-dresden.de/)

#ifndef CLAD_OPENCLGEN_H
#define CLAD_OPENCLGEN_H

#include "clang/AST/Decl.h"
#include "clang/AST/StmtVisitor.h"

namespace clad {

class OpenCLGen : public clang::ConstStmtVisitor<OpenCLGen, std::string> {

  clang::FunctionDecl *m_FD;

public:
  OpenCLGen(clang::FunctionDecl *FD) : m_FD(FD) {}
  std::string generate() {return "__global f {" + Visit(m_FD -> getBody()) + "}"; };
  void execute();
  std::string VisitBinaryOperator(const clang::BinaryOperator* BinOp);
  std::string VisitCompoundStmt(const clang::CompoundStmt* DS);

};
} // namespace clang

#endif // CLAD_OPENCLGEN_H
