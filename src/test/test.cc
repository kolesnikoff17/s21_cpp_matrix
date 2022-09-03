#include <gtest/gtest.h>

#include "../s21_matrix_oop.h"

class S21MatrixTest : public ::testing::Test {
 protected:
  S21Matrix m1;
  S21Matrix m2;

  void SetUp() {
    for (int i = 0; i < m1.get_rows(); ++i)
      for (int j = 0; j < m1.get_cols(); ++j) {
        m1(i, j) = (i + 1) * j + 1;
        m2(i, j) = (i + 1) * j + 1;
      }
  }
};

TEST(Other, RowsColsConstructorTest) {
  S21Matrix m1(2, 2);
  EXPECT_EQ(2, m1.get_cols());
  EXPECT_EQ(2, m1.get_rows());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j) EXPECT_DOUBLE_EQ(0, m1(i, j));

  ASSERT_ANY_THROW(S21Matrix m2(0, 0));
}

TEST(Other, CopyConstructorTest) {
  S21Matrix m1(2, 2);
  m1(0, 1) = 2;
  S21Matrix m2(m1);
  EXPECT_EQ(2, m2.get_cols());
  EXPECT_EQ(2, m2.get_rows());
  for (int i = 0; i < m2.get_rows(); ++i)
    for (int j = 0; j < m2.get_cols(); ++j)
      if (i == 0 && j == 1)
        EXPECT_DOUBLE_EQ(2, m1(0, 1));
      else
        EXPECT_DOUBLE_EQ(0, m1(i, j));
}

TEST(Other, DefaultConstructorTest) {
  S21Matrix m1;
  EXPECT_EQ(3, m1.get_cols());
  EXPECT_EQ(3, m1.get_rows());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j) EXPECT_DOUBLE_EQ(0, m1(i, j));
}

TEST_F(S21MatrixTest, ParenthesesOperatorTest) {
  m1(0, 0) = 10;
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      if (i == 0 && j == 0)
        EXPECT_DOUBLE_EQ(10, m1(0, 0));
      else
        EXPECT_DOUBLE_EQ((i + 1) * j + 1, m1(i, j));

  ASSERT_ANY_THROW(m1(5, 5));
  ASSERT_ANY_THROW(m1(-5, -5));
}

TEST_F(S21MatrixTest, SetRowsTest) {
  m1.set_rows(2);
  EXPECT_EQ(2, m1.get_rows());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ((i + 1) * j + 1, m1(i, j));

  m2.set_rows(4);
  EXPECT_EQ(4, m2.get_rows());
  for (int i = 0; i < m2.get_rows(); ++i)
    for (int j = 0; j < m2.get_cols(); ++j)
      if (i == 3)
        EXPECT_DOUBLE_EQ(0, m2(i, j));
      else
        EXPECT_DOUBLE_EQ((i + 1) * j + 1, m2(i, j));

  ASSERT_ANY_THROW(m1.set_rows(0));
}

TEST_F(S21MatrixTest, SetColsTest) {
  m1.set_cols(2);
  EXPECT_EQ(2, m1.get_cols());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ((i + 1) * j + 1, m1(i, j));

  m2.set_cols(4);
  EXPECT_EQ(4, m2.get_cols());
  for (int i = 0; i < m2.get_rows(); ++i)
    for (int j = 0; j < m2.get_cols(); ++j)
      if (j == 3)
        EXPECT_DOUBLE_EQ(0, m2(i, j));
      else
        EXPECT_DOUBLE_EQ((i + 1) * j + 1, m2(i, j));

  ASSERT_ANY_THROW(m1.set_cols(0));
}

TEST_F(S21MatrixTest, EqTest) {
  EXPECT_TRUE(m1.EqMatrix(m2));
  m2(0, 1) = 1;
  EXPECT_FALSE(m1.EqMatrix(m2));
}

TEST_F(S21MatrixTest, EqOperatorTest) {
  EXPECT_TRUE(m1 == m2);
  m2(0, 1) = 1;
  EXPECT_FALSE(m1 == m2);
}

TEST_F(S21MatrixTest, AddTest) {
  m1.SumMatrix(m2);
  S21Matrix m3(2, 2);
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m1(i, j));

  ASSERT_ANY_THROW(m1.SumMatrix(m3));
}

TEST_F(S21MatrixTest, AddAndAssignOperatorTest) {
  m1 += m2;
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m1(i, j));
}

TEST_F(S21MatrixTest, AddOperatorTest) {
  S21Matrix m3 = m1 + m2;
  for (int i = 0; i < m3.get_rows(); ++i)
    for (int j = 0; j < m3.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m3(i, j));
}

TEST_F(S21MatrixTest, SubTest) {
  m1.SubMatrix(m2);
  S21Matrix m3(2, 2);
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j) EXPECT_DOUBLE_EQ(0, m1(i, j));

  ASSERT_ANY_THROW(m1.SumMatrix(m3));
}

TEST_F(S21MatrixTest, SubAndAssignOperatorTest) {
  m1 -= m2;
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j) EXPECT_DOUBLE_EQ(0, m1(i, j));
}

TEST_F(S21MatrixTest, SubOperatorTest) {
  S21Matrix m3 = m1 - m2;
  for (int i = 0; i < m3.get_rows(); ++i)
    for (int j = 0; j < m3.get_cols(); ++j) EXPECT_DOUBLE_EQ(0, m3(i, j));
}

TEST_F(S21MatrixTest, MulNumberTest) {
  m1.MulNumber(2);
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m1(i, j));
}

TEST_F(S21MatrixTest, MulNumberAndAssignOperatorTest) {
  m1 *= 2;
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m1(i, j));
}

TEST_F(S21MatrixTest, MulNumberOperatorTest) {
  S21Matrix m3 = m1 * 2;
  for (int i = 0; i < m3.get_rows(); ++i)
    for (int j = 0; j < m3.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m3(i, j));

  S21Matrix m4 = 2 * m1;
  for (int i = 0; i < m4.get_rows(); ++i)
    for (int j = 0; j < m4.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(2 * ((i + 1) * j + 1), m4(i, j));
}

TEST_F(S21MatrixTest, MulMatrixTest) {
  m1.MulMatrix(m2);
  EXPECT_EQ(3, m1.get_cols());
  EXPECT_EQ(3, m1.get_rows());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      if (j == 0)
        EXPECT_DOUBLE_EQ(6 + 3 * i, m1(i, j));
      else if (j == 1)
        EXPECT_DOUBLE_EQ(20 + 11 * i, m1(i, j));
      else
        EXPECT_DOUBLE_EQ(34 + 19 * i, m1(i, j));

  S21Matrix m3(3, 1);
  ASSERT_ANY_THROW(m3.MulMatrix(m2));

  for (int i = 0; i < m3.get_rows(); ++i) m3(i, 0) = 2;
  m2.MulMatrix(m3);
  EXPECT_EQ(1, m2.get_cols());
  EXPECT_EQ(3, m2.get_rows());
  for (int i = 0; i < m2.get_rows(); ++i)
    EXPECT_DOUBLE_EQ(12 + 6 * i, m2(i, 0));
}

TEST_F(S21MatrixTest, MulMatrixAndAssignOperatorTest) {
  m1 *= m2;
  EXPECT_EQ(3, m1.get_cols());
  EXPECT_EQ(3, m1.get_rows());
  for (int i = 0; i < m1.get_rows(); ++i)
    for (int j = 0; j < m1.get_cols(); ++j)
      if (j == 0)
        EXPECT_DOUBLE_EQ(6 + 3 * i, m1(i, j));
      else if (j == 1)
        EXPECT_DOUBLE_EQ(20 + 11 * i, m1(i, j));
      else
        EXPECT_DOUBLE_EQ(34 + 19 * i, m1(i, j));
}

TEST_F(S21MatrixTest, MulMatrixOperatorTest) {
  S21Matrix m3 = m1 * m2;
  EXPECT_EQ(3, m3.get_cols());
  EXPECT_EQ(3, m3.get_rows());
  for (int i = 0; i < m3.get_rows(); ++i)
    for (int j = 0; j < m3.get_cols(); ++j)
      if (j == 0)
        EXPECT_DOUBLE_EQ(6 + 3 * i, m3(i, j));
      else if (j == 1)
        EXPECT_DOUBLE_EQ(20 + 11 * i, m3(i, j));
      else
        EXPECT_DOUBLE_EQ(34 + 19 * i, m3(i, j));
}

TEST_F(S21MatrixTest, TransponseTest) {
  m2 = m1.Transpose();
  for (int i = 0; i < m2.get_rows(); ++i)
    for (int j = 0; j < m2.get_cols(); ++j)
      EXPECT_DOUBLE_EQ(m1(j, i), m2(i, j));
}

TEST_F(S21MatrixTest, CalcComplementsTest) {
  m1(1, 1) = -20;
  m2 = m1.CalcComplements();
  EXPECT_DOUBLE_EQ(-160, m2(0, 0));
  EXPECT_DOUBLE_EQ(-2, m2(0, 1));
  EXPECT_DOUBLE_EQ(24, m2(0, 2));
  EXPECT_DOUBLE_EQ(-2, m2(1, 0));
  EXPECT_DOUBLE_EQ(4, m2(1, 1));
  EXPECT_DOUBLE_EQ(-2, m2(1, 2));
  EXPECT_DOUBLE_EQ(70, m2(2, 0));
  EXPECT_DOUBLE_EQ(-2, m2(2, 1));
  EXPECT_DOUBLE_EQ(-22, m2(2, 2));

  m2.set_cols(2);
  ASSERT_ANY_THROW(m2.CalcComplements());
}

TEST_F(S21MatrixTest, DeterminantTest) {
  m1(1, 1) = -20;
  double det = m1.Determinant();
  EXPECT_DOUBLE_EQ(det, -92);

  m2.set_cols(2);
  ASSERT_ANY_THROW(m2.Determinant());
}

TEST_F(S21MatrixTest, InverseMatrixTest) {
  m1(1, 1) = -20;
  S21Matrix m3 = m1.InverseMatrix();
  EXPECT_DOUBLE_EQ(40. / 23, m3(0, 0));
  EXPECT_DOUBLE_EQ(1. / 46, m3(0, 1));
  EXPECT_DOUBLE_EQ(-35. / 46, m3(0, 2));
  EXPECT_DOUBLE_EQ(1. / 46, m3(1, 0));
  EXPECT_DOUBLE_EQ(-1. / 23, m3(1, 1));
  EXPECT_DOUBLE_EQ(1. / 46, m3(1, 2));
  EXPECT_DOUBLE_EQ(-6. / 23, m3(2, 0));
  EXPECT_DOUBLE_EQ(1. / 46, m3(2, 1));
  EXPECT_DOUBLE_EQ(11. / 46, m3(2, 2));

  ASSERT_ANY_THROW(m2.InverseMatrix());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}