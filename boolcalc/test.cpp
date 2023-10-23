#include <string>
#include <gtest/gtest.h>

#include "boolexpr.h"

TEST(BoolExpr, cnf_test)
{
  BooleanExpression first("x1 + x2");
  BooleanExpression second("x1 < x2");
  BooleanExpression third("~x1");
  BooleanExpression fourth("x1 ^ x2");

  EXPECT_EQ(first.cnf().str(), "((x1 v x2) & (~x1 v ~x2))");
  EXPECT_EQ(second.cnf().str(), "(x1 v ~x2)");
  EXPECT_EQ(third.cnf().str(), "~x1");
  EXPECT_EQ(fourth.cnf().str(), "(((x1 v ~x2) & (~x1 v x2)) & (~x1 v ~x2))");
}

TEST(BoolExpr, dnf_test)
{
  BooleanExpression first("x1 + x2");
  BooleanExpression second("x1 < x2");
  BooleanExpression third("~x1");
  BooleanExpression fourth("x1 ^ x2");

  EXPECT_EQ(first.dnf().str(), "((~x1 & x2) v (x1 & ~x2))");
  EXPECT_EQ(second.dnf().str(), "(((~x1 & ~x2) v (x1 & ~x2)) v (x1 & x2))");
  EXPECT_EQ(third.dnf().str(), "~x1");
  EXPECT_EQ(fourth.dnf().str(), "(~x1 & ~x2)");
}

TEST(BoolExpr, zhegalkin_test)
{
  BooleanExpression first("~x1");
  
  EXPECT_EQ(first.zhegalkin().str(), "(x1 + 1)");
}

TEST(BoolExpr, isfull_test)
{
  std::vector<BooleanExpression> expression;
  expression.emplace_back("x1 + x2");
  expression.emplace_back("x1 < x2");
  expression.emplace_back("~x1");
  expression.emplace_back("x1 ^ x2");
  
  EXPECT_EQ(isFullSystem(expression), 1);
}
