/**
 * @file   MatrixInBasis_test.cpp
 * @author Jan Unsleber
 * @date   April 09. 2017
 * @copyright \n
 *  This file is part of the program Serenity.\n\n
 *  Serenity is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.\n\n
 *  Serenity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.\n\n
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with Serenity.
 *  If not, see <http://www.gnu.org/licenses/>.\n
 */
/* Include Serenity Internal Headers */
#include "data/matrices/MatrixInBasis.h"
#include "testsupply/BasisController__TEST_SUPPLY.h"
/* Include Std and External Headers */
#include <gtest/gtest.h>

namespace Serenity {

class MatrixInBasisTest : public ::testing::Test {
 protected:
  MatrixInBasisTest()
    : tBasisController(BasisController__TEST_SUPPLY::getBasisController(TEST_BASIS_CONTROLLERS::MINIMAL)),
      tr(tBasisController),
      tu(tBasisController) {
    tr(0, 0) = 0.0;
    tr(0, 1) = 0.1;
    tr(1, 0) = 1.0;
    tr(1, 1) = 1.1;
    tu.alpha(0, 0) = 0.0;
    tu.alpha(0, 1) = 0.1;
    tu.alpha(1, 0) = 1.0;
    tu.alpha(1, 1) = 1.1;
    tu.beta(0, 0) = 0.0;
    tu.beta(0, 1) = 0.1;
    tu.beta(1, 0) = 1.0;
    tu.beta(1, 1) = 1.1;
  }
  virtual ~MatrixInBasisTest() = default;
  // The basis in which it is defined
  std::shared_ptr<BasisController> tBasisController;
  // The object under test
  MatrixInBasis<RESTRICTED> tr;
  MatrixInBasis<UNRESTRICTED> tu;
};
/**
 * @test
 * @brief Tests MatrixInBasis.h: Normal construction.
 */
TEST_F(MatrixInBasisTest, BasisConstruct_R) {
  MatrixInBasis<RESTRICTED> matrix(tBasisController);
  // Check for zero initialization
  EXPECT_EQ(matrix(0, 0), 0.0);
  EXPECT_EQ(matrix(0, 1), 0.0);
  EXPECT_EQ(matrix(1, 0), 0.0);
  EXPECT_EQ(matrix(1, 1), 0.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Copy construction.
 */
TEST_F(MatrixInBasisTest, CopyConstruct_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  // Check for zero initialization
  EXPECT_EQ(matrix(0, 0), 0.0);
  EXPECT_EQ(matrix(0, 1), 0.1);
  EXPECT_EQ(matrix(1, 0), 1.0);
  EXPECT_EQ(matrix(1, 1), 1.1);
  EXPECT_EQ(tr(0, 0), 0.0);
  EXPECT_EQ(tr(0, 1), 0.1);
  EXPECT_EQ(tr(1, 0), 1.0);
  EXPECT_EQ(tr(1, 1), 1.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator= MatrixXd.
 */
TEST_F(MatrixInBasisTest, Eq_MatrixXd_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  Eigen::MatrixXd test(2, 2);
  test << 1.0, 2.0, 2.0, 3.0;
  matrix = test;
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(0, 1), 2.0);
  EXPECT_EQ(matrix(1, 0), 2.0);
  EXPECT_EQ(matrix(1, 1), 3.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator= MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, Eq_MatInB_1_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  MatrixInBasis<RESTRICTED> test(tBasisController);
  test(0, 0) = 1.0;
  test(1, 0) = 2.0;
  test(0, 1) = 2.0;
  test(1, 1) = 3.0;
  matrix = test;
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(0, 1), 2.0);
  EXPECT_EQ(matrix(1, 0), 2.0);
  EXPECT_EQ(matrix(1, 1), 3.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator= MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, Eq_Ref_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  matrix(0, 0) = 500.0;
  const auto& test = &matrix;
  EXPECT_EQ((*test)(0, 0), 500.0);
  EXPECT_EQ((*test)(0, 1), 0.1);
  EXPECT_EQ((*test)(1, 0), 1.0);
  EXPECT_EQ((*test)(1, 1), 1.1);
  EXPECT_EQ(test->getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator= MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, Eq_MatInB_2_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  auto bas2 = BasisController__TEST_SUPPLY::getBasisController(TEST_BASIS_CONTROLLERS::SMALL_MIXED);
  MatrixInBasis<RESTRICTED> test(bas2);
  EXPECT_THROW(matrix.operator=(test), SerenityError);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator= Eigen3 computed expression.
 */
TEST_F(MatrixInBasisTest, Eq_MatProd_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  Eigen::MatrixXd test1(2, 2);
  test1 << 1.0, 2.0, 2.0, 3.0;
  Eigen::MatrixXd test2(2, 2);
  test2 << 1.0, 0.0, 0.0, 1.0;
  matrix = test1 * test2;
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(0, 1), 2.0);
  EXPECT_EQ(matrix(1, 0), 2.0);
  EXPECT_EQ(matrix(1, 1), 3.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator+= MatrixXd.
 */
TEST_F(MatrixInBasisTest, PE_MatrixXd_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  Eigen::MatrixXd test(2, 2);
  test << 1.0, 2.0, 2.0, 3.0;
  matrix += test;
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(0, 1), 2.1);
  EXPECT_EQ(matrix(1, 0), 3.0);
  EXPECT_EQ(matrix(1, 1), 4.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator-= MatrixXd.
 */
TEST_F(MatrixInBasisTest, ME_MatrixXd_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  Eigen::MatrixXd test(2, 2);
  test << 1.0, 2.0, 2.0, 3.0;
  matrix -= test;
  EXPECT_EQ(matrix(0, 0), -1.0);
  EXPECT_EQ(matrix(0, 1), -1.9);
  EXPECT_EQ(matrix(1, 0), -1.0);
  EXPECT_EQ(matrix(1, 1), -1.9);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator+= MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, PE_MatInB_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  MatrixInBasis<RESTRICTED> test(tBasisController);
  test(0, 0) = 1.0;
  test(1, 0) = 2.0;
  test(0, 1) = 2.0;
  test(1, 1) = 3.0;
  matrix += test;
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(0, 1), 2.1);
  EXPECT_EQ(matrix(1, 0), 3.0);
  EXPECT_EQ(matrix(1, 1), 4.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator-= MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, ME_MatInB_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  MatrixInBasis<RESTRICTED> test(tBasisController);
  test(0, 0) = 1.0;
  test(1, 0) = 2.0;
  test(0, 1) = 2.0;
  test(1, 1) = 3.0;
  matrix -= test;
  EXPECT_EQ(matrix(0, 0), -1.0);
  EXPECT_EQ(matrix(0, 1), -1.9);
  EXPECT_EQ(matrix(1, 0), -1.0);
  EXPECT_EQ(matrix(1, 1), -1.9);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator* MatrixInBasis.
 */
TEST_F(MatrixInBasisTest, Multiply_MatInB_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  MatrixInBasis<RESTRICTED> test(tBasisController);
  test(0, 0) = 1.0;
  test(1, 0) = 2.0;
  test(0, 1) = 2.0;
  test(1, 1) = 3.0;
  auto result = matrix * test;
  EXPECT_EQ(result.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: operator* mixed.
 */
TEST_F(MatrixInBasisTest, Multiply_Mixed_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  MatrixInBasis<RESTRICTED> test1(tBasisController);
  test1(0, 0) = 1.0;
  test1(1, 0) = 2.0;
  test1(0, 1) = 2.0;
  test1(1, 1) = 3.0;
  Eigen::MatrixXd test2(2, 2);
  test2 << 1.0, 0.0, 0.0, 1.0;
  Eigen::MatrixXd result = test1 * test2;
  EXPECT_EQ(result(0, 0), 1.0);
  EXPECT_EQ(result(0, 1), 2.0);
  EXPECT_EQ(result(1, 0), 2.0);
  EXPECT_EQ(result(1, 1), 3.0);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: getBasisController(), getNBasisFunctions()
 */
TEST_F(MatrixInBasisTest, GetBasis_R) {
  MatrixInBasis<RESTRICTED> matrix(tBasisController);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
  EXPECT_EQ(matrix.getNBasisFunctions(), (unsigned int)2);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: total()
 */
TEST_F(MatrixInBasisTest, Total_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  auto total = matrix.total();
  EXPECT_EQ(total(0, 0), 0.0);
  EXPECT_EQ(total(0, 1), 0.1);
  EXPECT_EQ(total(1, 0), 1.0);
  EXPECT_EQ(total(1, 1), 1.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: difference().
 */
TEST_F(MatrixInBasisTest, Difference_R) {
  MatrixInBasis<RESTRICTED> matrix(tr);
  auto diff = matrix.difference();
  EXPECT_EQ(diff(0, 0), 0.0);
  EXPECT_EQ(diff(0, 1), 0.0);
  EXPECT_EQ(diff(1, 0), 0.0);
  EXPECT_EQ(diff(1, 1), 0.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Move Construction
 */
TEST_F(MatrixInBasisTest, MoveConstruct_R) {
  EXPECT_NE(tr.data(), nullptr);
  MatrixInBasis<RESTRICTED> result(std::move(tr));
  EXPECT_EQ(result(0, 0), 0.0);
  EXPECT_EQ(result(0, 1), 0.1);
  EXPECT_EQ(result(1, 1), 1.1);
  EXPECT_NE(tr.data(), result.data());
  EXPECT_EQ(result.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Move assignment operator
 */
TEST_F(MatrixInBasisTest, MoveAssignement_R) {
  MatrixInBasis<RESTRICTED> result(tBasisController);
  result = std::move(tr);
  EXPECT_EQ(result(0, 0), 0.0);
  EXPECT_EQ(result(0, 1), 0.1);
  EXPECT_EQ(result(1, 1), 1.1);
  EXPECT_NE(tr.data(), result.data());
  EXPECT_EQ(0.0, tr(0, 0));
  EXPECT_EQ(result.getBasisController(), tBasisController);
}

/**
 * @test
 * @brief Tests MatrixInBasis.h: to cout
 */
TEST_F(MatrixInBasisTest, Stream_R) {
  std::cout << tr << std::endl;
}

/* ==================
 *    UNRESTRICTED
 * ==================
 */

/**
 * @test
 * @brief Tests MatrixInBasis.h: Normal construction
 */
TEST_F(MatrixInBasisTest, BasisConstruct_U) {
  MatrixInBasis<UNRESTRICTED> matrix(tBasisController);
  // Check for zero initialization
  EXPECT_EQ(matrix.alpha(0, 0), 0.0);
  EXPECT_EQ(matrix.alpha(0, 1), 0.0);
  EXPECT_EQ(matrix.alpha(1, 0), 0.0);
  EXPECT_EQ(matrix.alpha(1, 1), 0.0);
  EXPECT_EQ(matrix.beta(0, 0), 0.0);
  EXPECT_EQ(matrix.beta(1, 0), 0.0);
  EXPECT_EQ(matrix.beta(0, 1), 0.0);
  EXPECT_EQ(matrix.beta(1, 1), 0.0);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Copy construction.
 */
TEST_F(MatrixInBasisTest, CopyConstruct_U) {
  MatrixInBasis<UNRESTRICTED> matrix(tu);
  // Check for zero initialization
  EXPECT_EQ(matrix.alpha(0, 0), 0.0);
  EXPECT_EQ(matrix.alpha(0, 1), 0.1);
  EXPECT_EQ(matrix.alpha(1, 0), 1.0);
  EXPECT_EQ(matrix.alpha(1, 1), 1.1);
  EXPECT_EQ(matrix.beta(0, 0), 0.0);
  EXPECT_EQ(matrix.beta(1, 0), 1.0);
  EXPECT_EQ(matrix.beta(0, 1), 0.1);
  EXPECT_EQ(matrix.beta(1, 1), 1.1);
  EXPECT_EQ(tu.alpha(0, 0), 0.0);
  EXPECT_EQ(tu.alpha(0, 1), 0.1);
  EXPECT_EQ(tu.alpha(1, 0), 1.0);
  EXPECT_EQ(tu.alpha(1, 1), 1.1);
  EXPECT_EQ(tu.beta(0, 0), 0.0);
  EXPECT_EQ(tu.beta(1, 0), 1.0);
  EXPECT_EQ(tu.beta(0, 1), 0.1);
  EXPECT_EQ(tu.beta(1, 1), 1.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: getBasisController(), getNBasisFunctions()
 */
TEST_F(MatrixInBasisTest, GetBasis_U) {
  MatrixInBasis<UNRESTRICTED> matrix(tBasisController);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
  EXPECT_EQ(matrix.getNBasisFunctions(), (unsigned int)2);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: total()
 */
TEST_F(MatrixInBasisTest, Total_U) {
  MatrixInBasis<UNRESTRICTED> matrix(tu);
  auto total = matrix.total();
  EXPECT_EQ(total(0, 0), 0.0);
  EXPECT_EQ(total(0, 1), 0.2);
  EXPECT_EQ(total(1, 0), 2.0);
  EXPECT_EQ(total(1, 1), 2.2);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: difference().
 */
TEST_F(MatrixInBasisTest, Difference_U) {
  MatrixInBasis<UNRESTRICTED> matrix(tu);
  matrix.beta.setZero();
  auto diff = matrix.difference();
  EXPECT_EQ(diff(0, 0), 0.0);
  EXPECT_EQ(diff(0, 1), 0.1);
  EXPECT_EQ(diff(1, 0), 1.0);
  EXPECT_EQ(diff(1, 1), 1.1);
  EXPECT_EQ(matrix.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Move assignment operator
 */
TEST_F(MatrixInBasisTest, MoveAssignement_U) {
  MatrixInBasis<UNRESTRICTED> result(tBasisController);
  result = std::move(tu);
  EXPECT_EQ(result.alpha(0, 0), 0.0);
  EXPECT_EQ(result.alpha(0, 1), 0.1);
  EXPECT_EQ(result.alpha(1, 1), 1.1);
  EXPECT_NE(tu.alpha.data(), result.alpha.data());
  EXPECT_EQ(0.0, tu.alpha(0, 0));
  EXPECT_EQ(result.getBasisController(), tBasisController);
}
/**
 * @test
 * @brief Tests MatrixInBasis.h: Move Construction
 */
TEST_F(MatrixInBasisTest, MoveConstruct_U) {
  EXPECT_NE(tu.alpha.data(), nullptr);
  MatrixInBasis<UNRESTRICTED> result(std::move(tu));
  EXPECT_EQ(result.alpha(0, 0), 0.0);
  EXPECT_EQ(result.alpha(0, 1), 0.1);
  EXPECT_EQ(result.alpha(1, 1), 1.1);
  EXPECT_NE(tu.alpha.data(), result.alpha.data());
  EXPECT_EQ(result.getBasisController(), tBasisController);
}

/**
 * @test
 * @brief Tests MatrixInBasis.h: to cout
 */
TEST_F(MatrixInBasisTest, Stream_U) {
  std::cout << tu << std::endl;
}

} /* namespace Serenity */
