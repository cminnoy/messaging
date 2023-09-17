#include <util/pmte.hpp>
#include <iostream>
#include <list>
#include <gtest/gtest.h>

struct class1 {
  int a;
};
struct unhandled {};

TEST(PMTE, EmptyVisitor) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  pmte_visitor visitor;

  auto v0 = build_pmte(5);
  auto v1 = make_pmte(6.0);
  auto v2 = make_unique_pmte(true);
  auto v3 = make_shared_pmte(class1 {});

  visitor((pmte_value *)nullptr);
  visitor(unique_pmte_value {});
  visitor(shared_pmte_value {});

  visitor(v0);
  visitor(v1);
  visitor(v2);
  visitor(v3);

  EXPECT_EQ(bool_counter, 0);
  EXPECT_EQ(int_counter, 0);
  EXPECT_EQ(double_counter, 0);
  EXPECT_EQ(class_counter, 0);

  delete v1;
}

TEST(PMTE, LambdaVisitor) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int float_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor;
  visitor
  .add<bool>  ([&](bool   v) { EXPECT_EQ(v, true); ++bool_counter; })
  .add<int>   ([&](int    v) { EXPECT_EQ(v, 5); ++int_counter; })
  .add        ([&](float  v) { EXPECT_EQ(v, 8.6f); ++float_counter; })
  .add<double>([&](double v) { EXPECT_EQ(v, 6.0); ++double_counter; })
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; })
  ;
  // clang-format on

  auto v0 = build_pmte(5);
  auto v1 = make_pmte(6.0);
  auto v2 = make_unique_pmte(true);
  auto v3 = make_shared_pmte(class1 {19});

  visitor(v0);
  ASSERT_EQ(int_counter, 1);
  visitor(v1);
  EXPECT_EQ(double_counter, 1);
  visitor(v2);
  EXPECT_EQ(bool_counter, 1);
  visitor(v3);
  EXPECT_EQ(class_counter, 1);
  visitor(make_unique_pmte(8.6f));
  EXPECT_EQ(float_counter, 1);
  visitor(make_shared_pmte(unhandled {}));

  visitor.remove<float>();
  visitor.remove<bool>();

  visitor(v0);
  ASSERT_EQ(int_counter, 2);
  visitor(v1);
  visitor(v2);
  visitor(v3);
  visitor(make_unique_pmte(8.6f));
  visitor(make_shared_pmte(unhandled {}));

  EXPECT_EQ(bool_counter, 1);
  EXPECT_EQ(int_counter, 2);
  EXPECT_EQ(float_counter, 1);
  EXPECT_EQ(double_counter, 2);
  EXPECT_EQ(class_counter, 2);

  delete v1;
}

#if __cplusplus >= CPP17_STANDARD
TEST(PMTE, ConstructorLambdaVisitor) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor(
    [&](bool   v) { EXPECT_EQ(v, true); ++bool_counter; },
    [&](int    v) { EXPECT_EQ(v, 5); ++int_counter; },
    [&](double v) { EXPECT_EQ(v, 6.0); ++double_counter; },
    [&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; }
  );
  // clang-format on

  auto v0 = build_pmte(5);
  auto v1 = make_pmte(6.0);
  auto v2 = make_unique_pmte(true);
  auto v3 = make_shared_pmte(class1 {19});

  visitor(v0);
  visitor(v1);
  visitor(v2);
  visitor(v3);

  EXPECT_EQ(bool_counter, 1);
  EXPECT_EQ(int_counter, 1);
  EXPECT_EQ(double_counter, 1);
  EXPECT_EQ(class_counter, 1);

  delete v1;
}
#endif

TEST(PMTE, DoubleAddRemove) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int float_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor;
  visitor
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; })
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 20); ++class_counter; })
  ;
  // clang-format on

  auto v0 = make_shared_pmte(class1 {19});
  auto v1 = make_shared_pmte(class1 {20});

  visitor(v1);

  visitor.remove<class1>();

  visitor(v0);

  EXPECT_EQ(bool_counter, 0);
  EXPECT_EQ(int_counter, 0);
  EXPECT_EQ(float_counter, 0);
  EXPECT_EQ(double_counter, 0);
  EXPECT_EQ(class_counter, 1);
}

TEST(PMTE, DoubleAddRemoveLast) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int float_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor;
  visitor
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; })
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 20); ++class_counter; })
  ;
  // clang-format on

  auto v0 = make_shared_pmte(class1 {19});
  auto v1 = make_shared_pmte(class1 {20});

  visitor(v1);

  visitor.remove_last<class1>();

  visitor(v0);

  EXPECT_EQ(bool_counter, 0);
  EXPECT_EQ(int_counter, 0);
  EXPECT_EQ(float_counter, 0);
  EXPECT_EQ(double_counter, 0);
  EXPECT_EQ(class_counter, 2);
}

TEST(PMTE, Visit) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int float_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor;
  visitor
  .add<bool>  ([&](bool   v) { EXPECT_EQ(v, true); ++bool_counter; })
  .add<int>   ([&](int    v) { EXPECT_EQ(v, 5); ++int_counter; })
  .add        ([&](float  v) { EXPECT_EQ(v, 8.6f); ++float_counter; })
  .add<double>([&](double v) { EXPECT_EQ(v, 6.0); ++double_counter; })
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; })
  ;
  // clang-format on

  auto v0 = build_pmte(5);
  auto v1 = make_pmte(6.0);
  auto v2 = make_unique_pmte(true);
  auto v3 = make_shared_pmte(class1 {19});

  visit(visitor, v0, v1, v2, v3, make_unique_pmte(8.6f), make_shared_pmte(unhandled {}));
  visit(visitor, v0, v1, v2, v3, make_unique_pmte(8.6f), make_shared_pmte(unhandled {}));

  visitor.remove<float>();
  visitor.remove<bool>();

  visit(visitor, v0, v1, v2, v3, make_unique_pmte(8.6f), make_shared_pmte(unhandled {}));
  visit(visitor, v0, v1, v2, v3, make_unique_pmte(8.6f), make_shared_pmte(unhandled {}));

  EXPECT_EQ(bool_counter, 2);
  EXPECT_EQ(int_counter, 4);
  EXPECT_EQ(float_counter, 2);
  EXPECT_EQ(double_counter, 4);
  EXPECT_EQ(class_counter, 4);

  delete v1;
}

TEST(PMTE, VisitContainer) {
  using namespace util::pmte;

  int bool_counter = 0;
  int int_counter = 0;
  int float_counter = 0;
  int double_counter = 0;
  int class_counter = 0;

  // clang-format off
  pmte_visitor visitor;
  visitor
  .add<bool>  ([&](bool   v) { EXPECT_EQ(v, true); ++bool_counter; })
  .add<int>   ([&](int    v) { EXPECT_EQ(v, 5); ++int_counter; })
  .add        ([&](float  v) { EXPECT_EQ(v, 8.6f); ++float_counter; })
  .add<double>([&](double v) { EXPECT_EQ(v, 6.0); ++double_counter; })
  .add<class1>([&](class1 v) { EXPECT_EQ(v.a, 19); ++class_counter; })
  ;
  // clang-format on

  std::list<unique_pmte_value> container;
  container.emplace_back(make_unique_pmte(5));
  container.emplace_back(make_unique_pmte(6.0));
  container.emplace_back(make_unique_pmte(true));
  container.emplace_back(make_unique_pmte(class1 {19}));
  container.emplace_back(make_unique_pmte(8.6f));
  container.emplace_back(make_unique_pmte(unhandled {}));

  visit(visitor, begin(container), end(container));
  visit(visitor, begin(container), end(container));

  visitor.remove<float>();
  visitor.remove<bool>();

  visit(visitor, begin(container), end(container));
  visit(visitor, begin(container), end(container));

  EXPECT_EQ(bool_counter, 2);
  EXPECT_EQ(int_counter, 4);
  EXPECT_EQ(float_counter, 2);
  EXPECT_EQ(double_counter, 4);
  EXPECT_EQ(class_counter, 4);
}

std::forward_list<util::pmte::shared_pmte_value> container;

template <typename Visitor>
void increment_doubles(Visitor vis) {
  vis.add([](double & v) { ++v; });
  util::pmte::visit(vis, begin(container), end(container));
}

TEST(PMTE, MoveConstructorVisitor) {
  container.emplace_front(util::pmte::make_shared_pmte<int>(5));
  container.emplace_front(util::pmte::make_shared_pmte<double>(8.8));
  util::pmte::pmte_visitor vis;
  vis.add([](int & v) { ++v; });
  util::pmte::visit(vis, begin(container), end(container));
  increment_doubles(std::move(vis));
  auto v0 = container.front();
  container.pop_front();
  auto v1 = container.front();
  container.pop_front();

  EXPECT_TRUE(util::pmte::is_type_of<double>(v0));
  EXPECT_TRUE(util::pmte::is_type_of<int>(v1));
  EXPECT_EQ(v0->get_unsafe<double>(), 9.8);
  EXPECT_EQ(v1->get_unsafe<int>(), 7);
}
