#include <gtest/gtest.h>

#include "metrics/average_metric.hpp"
#include "metrics/count_metric.hpp"
#include "metrics/registry.hpp"

using namespace metrics;

TEST(MetricsUnitTest, CountMetricBasic) {
  CountMetric<int> metric("count_test");
  metric.Record(1);
  metric.Record(2);
  metric.Record(3);

  EXPECT_EQ(metric.AggregateAndReset(), "6");
  EXPECT_EQ(metric.AggregateAndReset(), "0");
}

TEST(MetricsUnitTest, AverageMetricBasic) {
  AverageMetric<double> metric("avg_test");
  metric.Record(1.5);
  metric.Record(2.5);
  metric.Record(3.0);

  double avg = std::stod(metric.AggregateAndReset());
  EXPECT_NEAR(avg, 2.3333, 1e-4);

  EXPECT_EQ(std::stod(metric.AggregateAndReset()), 0.0);
}

TEST(MetricsUnitTest, RegistryRegistration) {
  auto& count = Registry::Instance().Register<CountMetric<int>>("reg_count");
  count.Record(10);

  auto& avg = Registry::Instance().Register<AverageMetric<double>>("reg_avg");
  avg.Record(5.0);
  avg.Record(15.0);

  auto metrics = Registry::Instance().GetMetrics();
  ASSERT_EQ(metrics.size(), 2u);
  EXPECT_EQ(metrics[0]->GetName(), "reg_count");
  EXPECT_EQ(metrics[1]->GetName(), "reg_avg");

  EXPECT_EQ(count.AggregateAndReset(), "10");
  EXPECT_EQ(std::stod(avg.AggregateAndReset()), 10.0);
}
