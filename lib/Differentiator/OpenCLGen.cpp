//--------------------------------------------------------------------*- C++ -*-
// clad - the C++ Clang-based Automatic Differentiator
// version: $Id$
// author:  Vassil Vassilev <vvasilev-at-cern.ch>
//------------------------------------------------------------------------------

#include <clad/Differentiator/OpenCLGen.h>

std::string clad::OpenCLGen::VisitBinaryOperator(const clang::BinaryOperator* BinOp)
{
    return "BO";
}

std::string clad::OpenCLGen::VisitCompoundStmt(const clang::CompoundStmt *Node) {
    std::string ret;
    for (clang::CompoundStmt::const_body_iterator i = Node->body_begin(),
         e = Node->body_end(); i != e; ++i)
    ret += Visit(*i) + ";";
    return ret;
}

