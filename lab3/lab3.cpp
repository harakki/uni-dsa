#include <iostream>

#define TREE_SIZE 5

using std::cin;
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

void array_create_inc(int array[])
{
    for (int i = 0; i < TREE_SIZE; ++i)
    {
        array[i] = i;
    }
}

void tree_sdp_double_cosv_create(Tree **root, int data)
{
    Tree **p = root;

    while (*p != nullptr)
    {
        if (data < (*p)->data)
        {
            p = &((*p)->left);
        }
        else if (data > (*p)->data)
        {
            p = &((*p)->right);
        }
        else
        {
            return;
        }
    }

    *p = new Tree(data);
}

void tree_sdp_delete_node(Tree *&root, int data)
{
    Tree **first_ptr = &root;

    while (*first_ptr != NULL)
    {
        if (data < (*first_ptr)->data)
            first_ptr = &((*first_ptr)->left);
        else if (data > ((*first_ptr)->data))
            first_ptr = &((*first_ptr)->right);
        else
            break;
    }

    if (*first_ptr != NULL)
    {
        Tree *second_ptr = *first_ptr;

        if ((second_ptr->left) == NULL)
            *first_ptr = second_ptr->right;
        else if ((second_ptr->right) == NULL)
            *first_ptr = second_ptr->left;
        else
        {
            Tree *third_ptr = second_ptr->left;
            Tree *fouth_ptr = second_ptr;

            if (third_ptr->right == NULL)
            {
                third_ptr->right = second_ptr->right;
                *first_ptr = third_ptr;
            }
            else
            {
                while ((third_ptr->right) != NULL)
                {
                    fouth_ptr = third_ptr;
                    third_ptr = third_ptr->right;
                }
                fouth_ptr->right = third_ptr->left;
                third_ptr->left = second_ptr->left;
                third_ptr->right = second_ptr->right;
                *first_ptr = third_ptr;
            }
        }

        delete second_ptr;
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

int main()
{
    Tree *root = nullptr;

    int gen_array[TREE_SIZE];
    int input_delete = 0;

    array_create_inc(gen_array);

    for (int i : gen_array)
    {
        tree_sdp_double_cosv_create(&root, i);
    }

    std::cout << "Случайное дерево поиска - удаление вершин\n\n";

    left_to_right(root);

    for (int i = 0; i < TREE_SIZE; ++i)
    {
        cout << "\nВведите данные для удаления: ";
        cin >> input_delete;

        tree_sdp_delete_node(root, input_delete);

        cout << "\nПосле удаления вершины:\n";
        left_to_right(root);
    }

    return 0;
}