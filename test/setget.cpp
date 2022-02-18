#include <CXXConfig.h>
#include <gtest/gtest.h>
using namespace cxxconfig;

class ConfigSetGet : public ::testing::TestWithParam<std::tuple<std::vector<unsigned int>, std::vector<unsigned int>>> {
};

TEST_P(ConfigSetGet, Default_Constructor_No_Throw) { ASSERT_NO_THROW(CXXConfig config); }

TEST_P(ConfigSetGet, Get_Reference_No_Throw) {
	CXXConfig config;
	config.set("key", 0);
	EXPECT_EQ(config.get_ref<int>("key"), 0);
}

INSTANTIATE_TEST_SUITE_P(Base, ConfigSetGet,
						 ::testing::Values(std::make_tuple(std::vector<unsigned int>{}, std::vector<unsigned int>{})));

template <class T> class ConfigSetGetType : public ::testing::Test {};
TYPED_TEST_SUITE_P(ConfigSetGetType);

TYPED_TEST_P(ConfigSetGetType, Default_Set_Get_Constructor_No_Throw) {
	CXXConfig config;
	ASSERT_NO_THROW(config.set("key", TypeParam()));
	ASSERT_NO_THROW(config.get<TypeParam>("key"));
}

REGISTER_TYPED_TEST_SUITE_P(ConfigSetGetType, Default_Set_Get_Constructor_No_Throw);
using ConfigSetGetRegisterTypes = ::testing::Types<uint32_t, uint16_t, float, double, long int>;
INSTANTIATE_TYPED_TEST_SUITE_P(ConfigRegister, ConfigSetGetType, ConfigSetGetRegisterTypes);