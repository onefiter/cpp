

#define RED 0
#define BLACK 1

typedef int KEY_TYPE;

/**
 * 定义一个RBTREE
 */
#define RBTREE_ENTRY(name, type)                                               \
  struct name {                                                                \
    struct type *right;                                                        \
    struct type *left;                                                         \
    struct type *parent;                                                       \
    unsigned char color;                                                       \
  }

typedef struct _rbtree_node {

  KEY_TYPE key;
  void *value;
#if 1

  struct _rbtree_node *right;
  struct _rbtree_node *left;
  // 做性质调整的辅助结点
  struct _rbtree_node *parent;

  unsigned char color;
#else
  RBTREE_ENTRY(, rb_node)
  node;

#endif
} rbtree_node;

typedef struct _rbtree {
  struct _rbtree_node *root;
  struct _rbtree_node *nil;

} rbtree;

// 基于x点旋转
void rbtree_left_rotate(rbtree *T, rbtree_node *x) {

  // 定义一个节点 y等于x的右子树
  rbtree_node *y = x->right;

  // 把x的右子树断开了,上一步已经把x的右子树赋值给了y
  x->right = x->left;
  if (y->left != T->nil) { // 判断是否是叶子节点
    y->left->parent = x;
  }

  // 2
  y->parent = x->parent;
  if (x->parent == T->nil) { // 初始的x是根节点
    T->root = y;
  } else if (x == x->parent->left) { // 初始的x节点是其父节点的左子树
    x->parent->left = y;
  } else { // 初始的x节点是其父节点的右子树
    x->parent->right = y;
  }

  // 3
  y->left = x;
  x->parent = y;
}

// 基于y点右旋转
void rbtree_right_rotate(rbtree *T, rbtree_node *y) {

  // 定义一个节点x等于y的左子树
  rbtree_node *x = y->left;

  // 把x的右子树断开了,上一步已经把x的右子树赋值给了y
  y->left = x->right;
  if (x->right != T->nil) { // 判断是否是叶子节点
    x->right->parent = y;
  }

  // 2
  x->parent = y->parent;
  if (y->parent == T->nil) { // 初始的y是根节点
    T->root = y;
  } else if (y == y->parent->right) { // 初始的y节点是其父节点的右子树
    y->parent->right = x;
  } else { // 初始的x节点是其父节点的右子树
    y->parent->left = x;
  }

  // 3
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *T, rbtree_node *z) {
  // z == red
  while (z->parent->color == RED) {

    if (z->parent == z->parent->parent->left) { // z的父节点是其祖父节点的左子树
      rbtree_node *y = z->parent->parent->right;

      if (y->color == RED) { // 其叔父节点是红色
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;

        z = z->parent->parent; // z === RED
      }
    } else { // y == BLACK z的叔父节点为黑色
      if (z == z->parent->left) {
        z->parent->color = BLACK;
        z->parent->parent->color = RED;

        rbtree_right_rotate(T, z->parent->parent);
      } else { // y == BLACK

        if (z == z->parent->right) {
          z = z->parent;
          rbtree_left_rotate(T, z);
        }

        z->parent->color = BLACK;
        z->parent->parent->color = RED;

        rbtree_right_rotate(T, z->parent->parent);
      }
    }

    // rbtree_node *y = z->parent->
  }
}

// 插入
// 插入都会插入的最底层
// 需要考虑循环的终止条件
void rbtree_insert(rbtree *T, rbtree_node *z) {
  rbtree_node *y = T->nil;
  rbtree_node *x = T->root;

  while (x != T->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else if (z->key > x->key) {
      x = x->right;
    } else { // Exit
      return;
    }
  }

  if (y->key > z->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  z->parent = y;
  z->left = T->nil;
  z->right = T->nil;
  z->color = RED;
}