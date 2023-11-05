#include <iostream>

#define TREE_SIZE 100

using std::cout;

struct Tree
{
    int data;

    Tree *left;
    Tree *right;

    Tree(int data) : data(data), left(nullptr), right(nullptr)
    {
    }
};

Tree *tree_isdp(int array[], int left, int right)
{
    if (left > right)
    {
        return 0;
    }

    int m = (left + right) / 2;

    Tree *root = new Tree(array[m]);

    root->left = tree_isdp(array, left, m - 1);
    root->right = tree_isdp(array, m + 1, right);

    return root;
}

void left_to_right(Tree *node)
{
    if (node != nullptr)
    {
        left_to_right(node->left);
        std::cout << node->data << " ⟩ ";
        left_to_right(node->right);
    }
}

int tree_size(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    int count_left = tree_size(node->left);
    int count_right = tree_size(node->right);

    return 1 + count_left + count_right;
}

int tree_checksum(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    int checksum_left = tree_checksum(node->left);
    int checksum_right = tree_checksum(node->right);

    return node->data + checksum_left + checksum_right;
}

float tree_height(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    float height_left = tree_height(node->left);
    float height_right = tree_height(node->right);

    return std::max(height_left, height_right) + 1;
}

float tree_height_sum(Tree *node, int depth)
{
    if (node == nullptr)
    {
        return 0;
    }

    float sum_left = tree_height_sum(node->left, depth + 1);
    float sum_right = tree_height_sum(node->right, depth + 1);

    return depth + sum_left + sum_right;
}

float tree_middle_height(Tree *root)
{
    return (float)(tree_height_sum(root, 1) / tree_size(root));
}

int main()
{
    Tree *root = nullptr;

    int array[TREE_SIZE];

    for (int i = 0; i < TREE_SIZE; i++)
    {
        array[i] = i;
    }

    int left = 0;
    int right = TREE_SIZE - 1;
    root = tree_isdp(array, left, right);

    std::cout << "Идеально сбалансированное дерево поиска\n\n";
    std::cout << "Обход дерева cлева направо:\n";
    left_to_right(root);
    cout << "\nРазмер дерева: " << tree_size(root) << "\n";
    cout << "Контрольная сумма: " << tree_checksum(root) << "\n";
    cout << "Высота: " << tree_height(root) << "\n";
    cout << "Средняя высота: " << tree_middle_height(root) << "\n";

    return 0;
}