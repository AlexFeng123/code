#include<iostream>
using namespace std;

/*state*/
struct Halt{};
struct Q0{};
struct Q1{};

/*Alphabet*/
struct Left{};
struct Right{};
struct A{};
struct Blank{};

/*tap representation*/
struct Nil{};
template<class Head, class Tail>
struct Pair{
   using head = Head;
   using tail = Tail;
};

/* transition function*/

template<typename State, typename Character>
struct TransitionFunction{};

/*q0 a -> (q1, #)*/
template<> struct TransitionFunction<Q0,A> {
 using next_state = Q1;
 using action = Blank;
};

/*q0 # -> (h, #)*/
template<> struct TransitionFunction<Q0,Blank> {
 using next_state = Halt;
 using action = Blank;
};

/*q1 a -> (q0, a)*/
template<> struct TransitionFunction<Q1,A> {
 using next_state = Q0;
 using action = A;
};
/*q1 # -> (q0, ->)*/
template<> struct TransitionFunction<Q1,Blank> {
 using next_state = Q0;
 using action = Right;
};

/*representation of a Configuration */
template<typename NextState, typename Action,
    typename Tape_Left, typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction;
template<typename State,
    typename Tape_Left,
    typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct Configuration {
     using next_state = typename Delta<State,Tape_Current>::next_state;
     using action = typename Delta<State,Tape_Current>::action;
     using halted_configuration = typename ApplyAction<next_state,action, Tape_Left,
                      Tape_Current,Tape_Right,Delta>::halted_configuration;

};

/*default action : write to current tape cell*/
template<typename NextState, typename Action,
    typename Tape_Left, typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction{
 using halted_configuration = typename Configuration<NextState,Tape_Left,Action,Tape_Right,
                            Delta>::halted_configuration;
};

/*move read head left*/
template<typename NextState, 
    typename Tape_Left, 
    typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction<NextState,Left,Tape_Left,Tape_Current,Tape_Right,Delta>{
 using halted_configuration = typename Configuration<NextState,
                            typename Tape_Left::tail,
                            typename Tape_Left::head,
                            Pair<Tape_Current,Tape_Right>,
                            Delta>::halted_configuration;
};

/*move read head right*/
template<typename NextState, 
    typename Tape_Left, 
    typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction<NextState,Right,Tape_Left,Tape_Current,Tape_Right,Delta>{
 using halted_configuration = typename  Configuration<NextState,
                            Pair<Tape_Current,Tape_Left>,
                            typename Tape_Right::head,
                            typename Tape_Right::tail,
                            Delta>::halted_configuration;
};


/*move read head right when there are no nonblank characters 
 to the right -- generate a new Blank symbol*/

template<typename NextState, 
    typename Tape_Left, 
    typename Tape_Current,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction<NextState,Right,Tape_Left,Tape_Current,Nil,Delta>{
 using halted_configuration = typename Configuration<NextState,
                            Pair<Tape_Current,Tape_Left>,
                            Blank,                                
                            Nil,
                            Delta>::halted_configuration;
};


/*we halt by not declaring a halted_configuration,
 * this causes the compiler to display an error message 
 * show the halting configuration*/
template<typename Action,
    typename Tape_Left, typename Tape_Current,
    typename Tape_Right,
    template<typename Q, typename Sigma> class Delta>
struct ApplyAction<Halt,Action,Tape_Left, Tape_Current,Tape_Right,Delta>{

};

int main()
{
    using Foo = typename Configuration<Q0,Nil,A,Pair<A,Pair<A,Nil>>,TransitionFunction>::halted_configuration;
    return 0;
}



