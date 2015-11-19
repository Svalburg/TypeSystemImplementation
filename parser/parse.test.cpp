#include <gtest/gtest.h>
#include "parse.h"
#include "export.h"

namespace {

struct ExpressionResult {
    bool m_success;
    int m_pos;
    bitpowder::lib::String m_errMsg;

    ExpressionResult() : m_success(false), m_pos(0), m_errMsg() {}
    ExpressionResult(bool success, int pos, bitpowder::lib::String errMsg) :
        m_success(success), m_pos(pos), m_errMsg(errMsg) {}
};

ExpressionResult setSourceExpression(XMASSource* source, const String& expr, MemoryPool& mp) {
    std::ostringstream errMsg;

    if (!source->valid()) {
        errMsg << "[XMASSource::setSourceExpression] parsing expression '"
               << expr
               << "' not attached: port is not connected.";
        std::cout << errMsg.str() << std::endl;
        return ExpressionResult(false, -1, errMsg.str());
    }

    auto result = ParseSourceExpression(expr, mp);
    // note: result has a bool() operator
    if (result) {
        //std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        // FIXME: Storing the values has a memory problem due to extensive use of MemoryPool (temporary memory).
        ClearMessageSpec(source);
        for (auto &packet : result.result().spec) {
            attachMessageSpec(&source->o, std::get<0>(packet).values, std::get<1>(packet));
        }
    }

    return ExpressionResult(result, result.position(), result.error());
}

std::string getSourceExpression(XMASSource* source) {
    StaticMemoryPool<128> mp;
    return Export(source, mp).stl();
}

#define EXPECT_CONNECTION(expected, origin, type, port) \
EXPECT_STREQ(expected, dynamic_cast<type *>(origin.get())->port.getTarget()->getName().stl().c_str())

TEST(TestNewXmas, reqrsp) {
    MemoryPool mp;

    auto reqrsp = ParseXMASFile("../test/reqrsp.xmas", mp);

    for (auto& kv : reqrsp) {
        // fprintf(stderr, "%s\n", kv.first.c_str());
        EXPECT_TRUE(kv.second->valid());
    }

    EXPECT_EQ(11, reqrsp.size());

    EXPECT_CONNECTION("Fork_0", reqrsp["Source_1"], XMASSource, o);
    EXPECT_CONNECTION("Merge_0", reqrsp["Source_0"], XMASSource, o);
    EXPECT_CONNECTION("Queue_0", reqrsp["Fork_0"], XMASFork, a);
    EXPECT_CONNECTION("Merge_0", reqrsp["Fork_0"], XMASFork, b);
    EXPECT_CONNECTION("Queue_1", reqrsp["Merge_0"], XMASMerge, o);

    EXPECT_CONNECTION("CtrlJoin_0", reqrsp["Switch_0"], XMASSwitch, a);

    EXPECT_CONNECTION("Sink_1", reqrsp["CtrlJoin_0"], XMASJoin, o);

    EXPECT_EQ(2, dynamic_cast<XMASQueue*>(reqrsp["Queue_0"].get())->c);
}

TEST(TestNewXmas, twoagents) {
    MemoryPool mp;

    auto reqrsp = ParseXMASFile("../test/twoagents.xmas", mp);

    // for (auto& kv : reqrsp) {
    //     fprintf(stderr, "%s\n", kv.first.c_str());
    // }

    EXPECT_EQ(56, reqrsp.size());
}

TEST(TestNewXmas, order) {
    MemoryPool mp;

    auto network = ParseXMASFile("../test/order.xmas", mp);

    EXPECT_EQ(13, network.size());
}

TEST(TestNewXmas, rational) {
    MemoryPool mp;

    auto network = ParseXMASFile("../test/rational.xmas", mp);

    EXPECT_EQ(12, network.size());
}

TEST(TestNewXmas, bluered) {
    MemoryPool mp;

    auto network = ParseXMASFile("../test/bluered.xmas", mp);

    EXPECT_EQ(10, network.size());
}

TEST(TestSourceSpec, replace_spec)
{
    XMASSink *m_sink;
    XMASSource *m_source;
    XMASFork *m_fork;
    XMASFunction *m_function;
    XMASJoin *m_join;
    XMASMerge *m_merge;
    XMASQueue *m_queue;

    m_source = new XMASSource("source");
    m_sink = new XMASSink("sink");
    m_fork = new XMASFork("fork");
    m_function = new XMASFunction("function");
    m_join = new XMASJoin("join");
    m_merge = new XMASMerge("merge");
    m_queue = new XMASQueue("queue");

    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::String result;

    /* connect port for valid spec attachment */
    connect(m_source->o, m_sink->i);
    ASSERT_TRUE(m_source->valid());
    ASSERT_TRUE(m_sink->valid());

    /* set specification to one string */

    ASSERT_NO_THROW(setSourceExpression(m_source, std::string("abc == 20"), mp));
    result = getSourceExpression(m_source);
    EXPECT_EQ("abc == 20", result.stl());

    /* overwrite specification with a larger string */
    ASSERT_NO_THROW(setSourceExpression(m_source, std::string("abc == 20 && def == 40"), mp));
    result = getSourceExpression(m_source);
    EXPECT_EQ("abc == 20 && def == 40", result.stl());

    /* overwrite specification with a smaller string */
    ASSERT_NO_THROW(setSourceExpression(m_source, std::string("def == 40"), mp));
    result = getSourceExpression(m_source);
    EXPECT_EQ("def == 40", result.stl());
}

}
