
#include "main.hpp"
#include <cstddef>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

std::unordered_map< std::string, Node >   mapNode;
std::unordered_map< size_t, std::string > mapNodeId;
std::vector< size_t >                     nodesOrder;
int                                       nodeId = 0;
std::vector< std::string >                outputNode;
std::vector< bool >                       nodeVal;

auto main( int /*argc*/, char *argv [] ) -> int
{
    string netList;
    string inputValue;
    netList    = argv [ 1 ];
    inputValue = argv [ 2 ];
    ifstream net( netList );
    string   gateLine;
    int      stage = 0;
    // 0 input
    // 1 output
    // 2 gates
    while ( getline( net, gateLine, '\n' ) )
    {
        if ( gateLine.size( ) <= 1 )
        {
            continue;
        }
        if ( gateLine [ 0 ] == '#' )
        {
            continue;
        }
        if ( ( stage == 0 ) && ( ( gateLine.find( "INPUT" ) == 0 ) || ( gateLine.find( "input" ) == 0 ) ) )
        {
            addInput( gateLine );
            continue;
        }
        if ( ( stage == 0 ) && ( ( gateLine.find( "OUTPUT" ) == 0 ) || ( gateLine.find( "output" ) == 0 ) ) )
        {
            addOutput( gateLine );
            stage = 1;
            continue;
        }
        if ( ( stage == 1 ) && ( ( gateLine.find( "OUTPUT" ) == 0 ) || ( gateLine.find( "output" ) == 0 ) ) )
        {
            addOutput( gateLine );
            continue;
        }
        stage = 2;
        addGate( gateLine );
    }
    net.close( );
    doSort( );
    eval( inputValue );
    print( );
    return 0;
}
auto getFromBracket( string st ) -> string
{
    bool   left  = false;
    bool   right = false;
    string name;
    for ( char i : st )
    {

        if ( i == '(' )
        {
            left = true;
            continue;
        }
        if ( i == ')' )
        {
            right = true;
            continue;
        }
        if ( left && ( ! right ) )
        {
            name += i;
        }
    }
    return name;
}
auto filterSpace( string st ) -> string
{

    size_t leftP = 0;
    for ( leftP = 0; leftP < st.length( ); leftP++ )
    {
        if ( st [ leftP ] != ' ' )
        {
            break;
        }
    }
    size_t rightP = 0;
    for ( rightP = st.length( ) - 1; rightP >= 0; rightP-- )
    {
        if ( st [ rightP ] != ' ' )
        {
            break;
        }
    }
    string name;
    for ( size_t i = leftP; i <= rightP; i++ )
    {
        name += st [ i ];
    }
    return name;
}
void addInput( string st )
{
    string name = filterSpace( getFromBracket( std::move( st ) ) );
    int    id   = nodeId;
    nodeId++;
    Node x;
    x.gateType      = INPUT;
    x.output        = id;
    x.outputName    = name;
    x.isWellDefined = true;
    mapNode.emplace( name, x );
}
void addOutput( string st )
{
    string name = filterSpace( getFromBracket( std::move( st ) ) );
    outputNode.emplace_back( name );
}
void addGate( string st )
{
    auto   equal      = st.find_first_of( '=' );
    auto   leftB      = st.find_first_of( '(' );
    auto   rightB     = st.find_first_of( ')' );
    string outputNode = filterSpace( st.substr( 0, equal ) );
    string gateType   = filterSpace( st.substr( equal + 1, leftB - equal - 1 ) );
    string inputNodes = getFromBracket( st.substr( leftB, rightB - leftB ) );
    Node   x;
    if ( mapNode.find( outputNode ) != mapNode.end( ) )
    {
        x = mapNode.find( outputNode )->second;
    }
    else
    {
        x.output = nodeId;
        nodeId++;
    }
    x.gateType                        = getGateType( gateType );
    x.outputName                      = outputNode;
    auto                  inputNodesV = getFromComma( inputNodes );
    std::vector< size_t > ans;
    for ( auto i : inputNodesV )
    {
        if ( mapNode.find( i ) != mapNode.end( ) )
        {
            ans.emplace_back( mapNode.find( i )->second.output );
        }
        else
        {
            Node temp;
            temp.outputName = i;
            temp.output     = nodeId;
            nodeId++;
            ans.emplace_back( temp.output );
            mapNode.emplace( i, temp );
        }
    }
    x.input         = ans;
    x.isWellDefined = true;
    if ( mapNode.find( outputNode ) != mapNode.end( ) )
    {
        mapNode [ outputNode ] = x;
    }
    else
    {
        mapNode.emplace( outputNode, x );
    }
}

auto getGateType( string st ) -> GateType
{
    if ( ( st.find( "xnor" ) != std::string::npos ) || ( st.find( "XNOR" ) != std::string::npos ) )
    {
        return XNOR;
    }
    if ( ( st.find( "xor" ) != std::string::npos ) || ( st.find( "XOR" ) != std::string::npos ) )
    {
        return XOR;
    }
    if ( ( st.find( "nor" ) != std::string::npos ) || ( st.find( "NOR" ) != std::string::npos ) )
    {
        return NOR;
    }
    if ( ( st.find( "or" ) != std::string::npos ) || ( st.find( "OR" ) != std::string::npos ) )
    {
        return OR;
    }
    if ( ( st.find( "nand" ) != std::string::npos ) || ( st.find( "NAND" ) != std::string::npos ) )
    {
        return NAND;
    }
    if ( ( st.find( "and" ) != std::string::npos ) || ( st.find( "AND" ) != std::string::npos ) )
    {
        return AND;
    }
    if ( ( st.find( "not" ) != std::string::npos ) || ( st.find( "NOT" ) != std::string::npos ) )
    {
        return INVERTER;
    }
    if ( ( st.find( "buf" ) != std::string::npos ) || ( st.find( "BUF" ) != std::string::npos ) )
    {
        return BUFFER;
    }
    cout << "ERROR" << endl;
    return BUFFER;
}
auto getFromComma( string st ) -> vector< string >
{
    vector< string > ans;
    string           str = std::move( st );
    while ( true )
    {
        auto pos = str.find_first_of( ',' );
        if ( pos == std::string::npos )
        {
            ans.push_back( filterSpace( str ) );
            return ans;
        }
        ans.push_back( filterSpace( str.substr( 0, pos ) ) );
        str = str.substr( pos + 1 );
    }
}
void doSort( )
{
    vector< std::size_t > nowInput;
    struct
    {
        auto operator( )( const pair< size_t, size_t > l, const pair< size_t, size_t > r ) const -> bool
        {
            return l.first > r.first;
        }
    } customLess;
    // < nodeInNum,nodeId>
    priority_queue< pair< size_t, size_t >, vector< pair< size_t, size_t > >, decltype( customLess ) > remain;
    vector< bool >                                                                                     visited;
    vector< vector< size_t > >                                                                         path;
    visited.resize( nodeId + 1 );
    nowInput.resize( nodeId + 1 );
    path.resize( nodeId + 1 );
    for ( auto i : mapNode )
    {
        remain.emplace( i.second.input.size( ), i.second.output );
        visited [ i.second.output ] = false;
        mapNodeId.emplace( i.second.output, i.second.outputName );
        nowInput [ i.second.output ] = i.second.input.size( );
        for ( auto x : i.second.input )
        {
            path [ x ].push_back( i.second.output );
        }
    }
    while ( ! remain.empty( ) )
    {
        auto x = remain.top( );
        remain.pop( );
        if ( visited [ x.second ] )
        {
            continue;
        }
        nodesOrder.push_back( x.second );
        visited [ x.second ] = true;
        for ( auto i : path [ x.second ] )
        {
            nowInput [ i ]--;
            remain.emplace( nowInput [ i ], i );
        }
    }
}
void eval( string st )
{
    ifstream valF( st );
    string   gateLine;
    nodeVal.resize( nodeId + 1 );
    while ( getline( valF, gateLine, '\n' ) )
    {
        auto   x                            = gateLine.find( ' ' );
        string name                         = filterSpace( gateLine.substr( 0, x ) );
        bool   val                          = stoi( filterSpace( gateLine.substr( x ) ) ) != 0;
        nodeVal [ mapNode [ name ].output ] = val;
    }
    valF.close( );
    for ( auto i : nodesOrder )
    {
        auto type   = mapNode [ mapNodeId [ i ] ].gateType;
        auto inputs = mapNode [ mapNodeId [ i ] ].input;
        auto output = mapNode [ mapNodeId [ i ] ].output;
        switch ( type )
        {
            case INPUT:
                break;
            case BUFFER:
            {
                nodeVal [ output ] = nodeVal [ inputs [ 0 ] ];
                break;
            }
            case INVERTER:
            {
                nodeVal [ output ] = ! nodeVal [ inputs [ 0 ] ];
                break;
            }
            case AND:
            {
                nodeVal [ output ] = true;
                for ( auto x : inputs )
                {
                    if ( ! nodeVal [ x ] )
                    {
                        nodeVal [ output ] = false;
                        break;
                    }
                }
                break;
            }
            case OR:
            {
                nodeVal [ output ] = false;
                for ( auto x : inputs )
                {
                    if ( nodeVal [ x ] )
                    {
                        nodeVal [ output ] = true;
                        break;
                    }
                }
                break;
            }
            case NAND:
            {
                nodeVal [ output ] = false;
                for ( auto x : inputs )
                {
                    if ( ! nodeVal [ x ] )
                    {
                        nodeVal [ output ] = true;
                        break;
                    }
                }
                break;
            }
            case NOR:
            {
                nodeVal [ output ] = true;
                for ( auto x : inputs )
                {
                    if ( nodeVal [ x ] )
                    {
                        nodeVal [ output ] = false;
                        break;
                    }
                }
                break;
            }
            case XOR:
            {
                nodeVal [ output ] = nodeVal [ inputs [ 0 ] ] xor nodeVal [ inputs [ 1 ] ];
                for ( size_t i = 2; i < inputs.size( ); i++ )
                {
                    nodeVal [ output ] = nodeVal [ output ] xor nodeVal [ inputs [ i ] ];
                }
                break;
            }
            case XNOR:
            {
                nodeVal [ output ] = nodeVal [ inputs [ 0 ] ] == nodeVal [ inputs [ 1 ] ];
                for ( size_t i = 2; i < inputs.size( ); i++ )
                {
                    nodeVal [ output ] = nodeVal [ output ] == nodeVal [ inputs [ i ] ];
                }
                break;
            }
        }
    }
}
void print( )
{
    for ( const auto &x : outputNode )
    {
        cout << x << " " << ( nodeVal [ mapNode [ x ].output ] ? 1 : 0 ) << endl;
    }
    return;
}