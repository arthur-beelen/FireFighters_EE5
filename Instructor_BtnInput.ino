 # i n c l u d e   < N e x t i o n . h > 
 
 N e x B u t t o n   o k B u t t o n   =   N e x B u t t o n ( 0 ,   5 ,   " o k B u t t o n " ) ;   / / ( p a g e ,   i d ,   o b j e c t N a m e ) 
 N e x P a g e   p a g e 0   =   N e x P a g e ( 0 ,   0 ,   " p a g e 0 " ) ; 
 i n t   t o u c h e d   =   0 ; 
 s t r i n g   b t n N a m e s [ 5 ]   =   { " d o w n . v a l = " ,   " r i g h t . v a l = " ,   " o k . v a l = " ,   " l e f t . v a l = " ,   " u p . v a l = " } ; 
 
 N e x T o u c h   * n e x _ l i s t e n _ l i s t [ ]   = 
 { 
     & o k B u t t o n , 
     & p a g e 0 , 
     N U L L 
 } ;     / /   E n d   o f   t o u c h   e v e n t   l i s t 
 
 v o i d   o k B u t t o n P u s h C a l l b a c k ( v o i d   * p t r ) { 
     d i g i t a l W r i t e ( A 1 ,   H I G H ) ; 
     t o u c h e d   =   1 ; 
 } 
 
 v o i d   o k B u t t o n P o p C a l l b a c k ( v o i d   * p t r ) { 
     d i g i t a l W r i t e ( A 1 ,   L O W ) ; 
     t o u c h e d     =   0 ; 
 } 
 
 v o i d   s e t u p ( )   { 
     p i n M o d e ( 3 ,   I N P U T ) ; 
     p i n M o d e ( 4 ,   I N P U T ) ; 
     p i n M o d e ( 5 ,   I N P U T ) ; 
     p i n M o d e ( A 5 ,   I N P U T ) ; 
     p i n M o d e ( A 0 ,   I N P U T ) ; 
     p i n M o d e ( A 1 ,   O U T P U T ) ; 
 
     S e r i a l . b e g i n ( 9 6 0 0 ) ; 
     n e x I n i t ( ) ; 
 
     o k B u t t o n . a t t a c h P u s h ( o k B u t t o n P u s h C a l l b a c k ,   & o k B u t t o n ) ; 
     o k B u t t o n . a t t a c h P o p ( o k B u t t o n P o p C a l l b a c k ,   & o k B u t t o n ) ; 
 } 
 
 v o i d   l o o p ( )   { 
     n e x L o o p ( n e x _ l i s t e n _ l i s t ) ; 
 
     b o o l   i n p u t S t a t e s [ 5 ] ; 
     c h a r   d i s p l a y C o m   [ 6 4 ] ; 
 
     / / N a k i j k e n   o f   i n p u t s t a t e s   e n   b t n N a m e s   o v e r e e n   k o m e n 
     i n p u t S t a t e s [ 0 ]   =   d i g i t a l R e a d ( 3 ) ; 
     i n p u t S t a t e s [ 1 ]   =   d i g i t a l R e a d ( 4 ) ; 
     i n p u t S t a t e s [ 2 ]   =   d i g i t a l R e a d ( 5 ) ; 
     i n p u t S t a t e s [ 3 ]   =   d i g i t a l R e a d ( A 0 ) ; 
     i n p u t S t a t e s [ 4 ]   =   d i g i t a l R e a d ( A 5 ) ; 
 
     f o r ( i n t   i   =   0 ;   i   <   5 ;   i + + ) { 
         s p r i n t f   ( d i s p l a y C o m ,   " % s % b " ,   b t n N a m e s [ i ] ,   i n p u t S t a t e s [ i ] ) ; 
         S e r i a l . p r i n t ( d i s p l a y C o m ) ; 
         S e r i a l . w r i t e ( 0 x f f ) ; 
         S e r i a l . w r i t e ( 0 x f f ) ; 
         S e r i a l . w r i t e ( 0 x f f ) ; 
     } 
 } 
