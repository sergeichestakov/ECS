        #ifndef _BINARY_HEAP_H_
        #define _BINARY_HEAP_H_

        // BinaryHeap class
        //
        // CONSTRUCTION: with a negative infinity sentinel and
        //               optional capacity (that defaults to 100)
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // deleteMin( minItem )   --> Remove (and optionally return) smallest item
        // Comparable findMin( )  --> Return smallest item
        // bool isEmpty( )        --> Return true if empty; else false
        // bool isFull( )         --> Return true if full; else false
        // void makeEmpty( )      --> Remove all items
        // ******************ERRORS********************************
        // Throws Underflow and Overflow as warranted

        template <class Comparable>
        class BinaryHeap
        {
          public:
            int **table;
            explicit BinaryHeap( int capacity = 100,  int col = 2);
            ~BinaryHeap() { delete array; }

            bool isEmpty( ) const;
            bool isFull( ) const;
            const Comparable & findMin( ) const;

            void insert( const Comparable & x );
            void deleteMin( );
            void deleteMin( Comparable & minItem );
            void makeEmpty( );

          private:
            int tableCol;
            int currentSize;  // Number of elements in heap
            int maxSize;
            Comparable *array;        // The heap array

            void buildHeap( );
            void percolateDown( int hole );
            void percolateUp( int hole );
            void percolateEmptyDown( int hole );
        };

        #include "BinaryHeap.cpp"
        #endif
