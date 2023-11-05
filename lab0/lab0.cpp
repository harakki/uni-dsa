#include <iostream>

struct Tree
{
    int data;

    Tree *left;
    Tree *right;

    Tree(int data) : data(data), left(nullptr), right(nullptr)
    {
    }
};

void print_tree(Tree *node)
{
    printf("    %.02i\n   /  \\\n  %.02i   %.02i\n /  \\   \\\n%.02i  %.02i   %.02i\n", node->data, node->left->data,
           node->right->data, node->left->left->data, node->left->right->data, node->right->right->data);
}

void up_to_down(Tree *node)
{
    if (node)
    {
        std::cout << node->data << " ⟩ ";
        up_to_down(node->left);
        up_to_down(node->right);
    }
}

void left_to_right(Tree *node)
{
    if (node)
    {
        left_to_right(node->left);
        std::cout << node->data << " ⟩ ";
        left_to_right(node->right);
    }
}

void down_to_up(Tree *node)
{
    if (node)
    {
        down_to_up(node->left);
        down_to_up(node->right);
        std::cout << node->data << " ⟩ ";
    }
}

int tree_size(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    else
    {
        int count_left = tree_size(node->left);
        int count_right = tree_size(node->right);

        return 1 + count_left + count_right;
    }
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
    Tree *root = new Tree(4);
    root->left = new Tree(2);
    root->left->left = new Tree(1);
    root->left->right = new Tree(3);
    root->right = new Tree(5);
    root->right->right = new Tree(6);

    std::cout << "Двоичное дерево\n\n";

    print_tree(root);

    std::cout << "\nОбходы дерева:\n";
    std::cout << "Сверху вниз \n";
    up_to_down(root);
    std::cout << "\nСлева направо \n";
    left_to_right(root);
    std::cout << "\nСнизу вверх \n";
    down_to_up(root);

    std::cout << "\n\n";

    std::cout << "Размер дерева: " << tree_size(root) << "\n";
    std::cout << "Контрольная сумма: " << tree_checksum(root) << "\n";
    std::cout << "Высота: " << tree_height(root) << "\n";
    std::cout << "Средняя высота: " << tree_middle_height(root) << "\n";

    return 0;
}