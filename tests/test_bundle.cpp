#include <util/bundle.hpp>

#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

struct Tree1 : public std::enable_shared_from_this<Tree1> {
  virtual ~Tree1() noexcept {}
};
struct Tree2 : public std::enable_shared_from_this<Tree2> {
  virtual ~Tree2() noexcept {}
};
struct Tree3 : public std::enable_shared_from_this<Tree3> {
  virtual ~Tree3() noexcept {}
};
struct Tree4 : public std::enable_shared_from_this<Tree4> {
  virtual ~Tree4() noexcept {}
};

struct Derived1 : public Tree1 {
  using Tree1::Tree1;
};

TEST(BundleTest, Create) {
  // Note: we can't test the static_asserts in util::bundle to be correct as the compile will always trigger them (so not SFINEA tricks to get around that)

  // Check if bundles can be default constructed
  {
    EXPECT_EQ((std::is_default_constructible<util::bundle<Tree1>>::value), 1);
    EXPECT_EQ((std::is_default_constructible<util::bundle<Tree2>>::value), 1);
    EXPECT_EQ((std::is_default_constructible<util::bundle<Tree1, Tree2>>::value), 1);
    EXPECT_EQ((std::is_default_constructible<util::bundle<Tree1, Tree2, Tree3>>::value), 1);
    EXPECT_EQ((std::is_default_constructible<util::bundle<Tree1, Tree2, Tree3, Tree4>>::value), 1);

    util::bundle<Tree1> b1;
    util::bundle<Tree1, Tree2> b2;
    util::bundle<Tree1, Tree2, Tree3> b3;
    util::bundle<Tree1, Tree2, Tree3, Tree4> b4;
  }

  // Check for assignment from shared pointers
  {
    std::shared_ptr<Tree1> s1 = std::make_shared<Tree1>();
    std::shared_ptr<Tree2> s2 = std::make_shared<Tree2>();
    std::shared_ptr<Tree3> s3 = std::make_shared<Tree3>();
    std::shared_ptr<Tree4> s4 = std::make_shared<Tree4>();

    std::shared_ptr<Derived1> d1 = std::make_shared<Derived1>();

    util::bundle<Tree1> b1(s1);
    util::bundle<Tree1, Tree2> b2(s1, s2);
    util::bundle<Tree1, Tree2> b3(d1, s2);
    util::bundle<Tree1, Tree2, Tree3> b4(s1, s2, s3);
    util::bundle<Tree1, Tree2, Tree3, Tree4> b5(s1, s2, s3, s4);
  }

  // {
  //     struct Tree1 : public std::enable_shared_from_this<Tree1> {
  //         virtual ~Tree1() noexcept {}
  //         void foo() { std::cout << "foo\n"; }
  //     };

  //     struct Tree2 : public std::enable_shared_from_this<Tree2> {
  //         virtual ~Tree2() noexcept {}
  //     };

  //     struct Tree3 : public std::enable_shared_from_this<Tree3> {
  //         virtual ~Tree3() noexcept {}
  //         virtual void hello_world() { std::cout << "Hello World!\n"; }
  //     };

  //     struct Child1 : Tree1 {
  //         using Tree1::Tree1;
  //     };

  //     struct Child2 : Tree2 {
  //         using Tree2::Tree2;
  //         void bar() { std::cout << "bar\n"; }
  //     };

  //     struct Child3 : Tree3 {
  //         using Tree3::Tree3;
  //         void foobar() { std::cout << "foobar\n"; }
  //         void hello_world() override { std::cout << "HELLO WORLD!\n"; }
  //     };

  //     util::bundle<Child1,Child2,Child3> source_bundle {
  //         std::make_shared<Child1>(),
  //         std::make_shared<Child2>(),
  //         std::make_shared<Child3>()
  //     };

  //     util::bundle<Tree1,Tree2,Tree3> base_bundle = source_bundle; // automatic static cast
  //     if (base_bundle) {
  //         base_bundle.get<0>()->foo();
  //         base_bundle.get<2>()->hello_world();
  //     }

  //     util::bundle<Tree1,Child2,Tree3> mixed_bundle = base_bundle; // automatic dynamic cast
  //     mixed_bundle()
  //     .then([](auto &b) {
  //         std::cout << "Yeah, I'm a winner!\n";
  //         b.template get<1>()->bar();
  //     }).or_else([](auto &) {
  //         std::cout << "Not for me :-(\n";
  //     });
  // }
}

int main(int argc, char * argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
