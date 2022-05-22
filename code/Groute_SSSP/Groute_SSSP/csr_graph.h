#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <cstdint>
#include<set>
using namespace std;
typedef unsigned int uint32_t;
typedef uint32_t index_t;
struct CSRGraphBase
{
    index_t nnodes, nedges;
    index_t* row_start; //ָ�루���࣬���ڷ��ʣ�
    index_t* edge_dst;  //ָ�루���࣬���ڷ��ʣ�

    CSRGraphBase(index_t nnodes, index_t nedges) :
        nnodes(nnodes), nedges(nedges), row_start(nullptr), edge_dst(nullptr) { }
    CSRGraphBase() :
        nnodes(0), nedges(0), row_start(nullptr), edge_dst(nullptr) { }
};

struct CSRGraph :public CSRGraphBase {
    std::vector<index_t> row_start_vec; //��ƫ����ʼ����ֵΪ���� + 1
    std::vector<index_t> edge_dst_vec;  //��ƫ��
    index_t* edge_weights;				//��Ȩֵ
    index_t* node_weights;				//�ڵ�Ȩֵ
    std::vector<index_t> edge_weights_vec; //��Ȩֵ
    //���캯��������Ϊ�ڵ������ͱ�����
    CSRGraph(index_t nnodes, index_t nedges) :
        CSRGraphBase(nnodes, nedges), row_start_vec(nnodes + 1), edge_dst_vec(nedges),
        edge_weights(nullptr), node_weights(nullptr)
    {
        row_start = &row_start_vec[0];  //����ָ��
        edge_dst = &edge_dst_vec[0];     //����ָ��
    }
    //�޲ι��캯��
    CSRGraph() : edge_weights(nullptr), node_weights(nullptr) { }
    //��������
    ~CSRGraph(){}
    //��ֵ����CSRGraph����������и�ֵ
    void Move(index_t nnodes, index_t nedges,vector<index_t>& row_start, vector<index_t>& edge_dst){
        this->nnodes = nnodes;
        this->nedges = nedges;
        this->row_start_vec = std::move(row_start);
        this->edge_dst_vec = std::move(edge_dst);
        this->row_start = &this->row_start_vec[0];
        this->edge_dst = &this->edge_dst_vec[0];
    }
    //��Ȩֵ��ֵ
    void MoveWeights(std::vector<index_t>& edge_weights){
        this->edge_weights_vec = std::move(edge_weights);
        this->edge_weights = &this->edge_weights_vec[0];
    }
    //����Ȩֵ
    void AllocWeights(){
        //vector::resize() : ���������Ĵ�С��ʹ�����ǰn��Ԫ�أ�ɾ�������±귶Χ��Ԫ�أ���ʼ������ָ������ԭ�ߴ��Ԫ��
        this->edge_weights_vec.resize(nedges);
        this->edge_weights = &this->edge_weights_vec[0];
    }
    //����ͼ�ṹ
    void Bind(index_t nnodes, index_t nedges,index_t* row_start, index_t* edge_dst,index_t* edge_weights, index_t* node_weights){
        this->nnodes = nnodes;
        this->nedges = nedges;

        this->row_start_vec.clear();    // ����
        this->edge_dst_vec.clear();     //����

        this->row_start = row_start;
        this->edge_dst = edge_dst;

        this->edge_weights = edge_weights;
        this->node_weights = node_weights;
    }
    //���������
    index_t max_degree() const{
        index_t max_degree = 0;
        for (index_t node = 0; node < nnodes; node++)
        {
            max_degree = max(max_degree, end_edge(node) - begin_edge(node));
        }
        return max_degree;
    }
    //��ǰ�ڵ�ĵ�һ���ߵ��±�
    index_t begin_edge(index_t node) const{
        return row_start[node];
    }
    //��ǰ�ڵ�����һ���ߵ��±� + 1
    index_t end_edge(index_t node) const{
        return row_start[node + 1];
    }
    index_t edge_dest(index_t edge) const{
        return edge_dst[edge];
    }
};

//����ģʽ������CSRGraph����
class CSRGraphGenerator{
private:
    index_t m_nnodes;
    int m_gen_factor;
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_nneighbors_distribution;
    std::uniform_int_distribution<index_t> m_node_distribution;

    int GenNeighborsNum(index_t node) { return m_nneighbors_distribution(m_generator); }
    index_t GenNeighbor(index_t node, std::set<index_t>& neighbors)
    {
        index_t neighbor;
        do
        {
            neighbor = m_node_distribution(m_generator);
        } while (neighbor == node || neighbors.find(neighbor) != neighbors.end());

        neighbors.insert(neighbor);
        return neighbor;
    }

public:
    CSRGraphGenerator(index_t nnodes, int gen_factor) :
        m_nnodes(nnodes), m_gen_factor(gen_factor), m_nneighbors_distribution(1, gen_factor), m_node_distribution(0, nnodes - 1)
    {
        assert(nnodes > 1);
        assert(gen_factor >= 1);
    }

    void Gen(CSRGraph& graph)
    {
        std::vector<index_t> row_start(m_nnodes + 1, 0);
        std::vector<index_t> edge_dst;
        edge_dst.reserve(m_nnodes * m_gen_factor); // approximation  

        for (index_t node = 0; node < m_nnodes; ++node)
        {
            row_start[node] = edge_dst.size();
            int nneighbors = GenNeighborsNum(node);
            std::set<index_t> neighbors;
            for (int i = 0; i < nneighbors; ++i)
            {
                edge_dst.push_back(GenNeighbor(node, neighbors));
            }
        }

        index_t nedges = edge_dst.size();
        row_start[m_nnodes] = nedges;

        edge_dst.shrink_to_fit(); // 

        graph.Move(m_nnodes, nedges, row_start, edge_dst);
    }
};