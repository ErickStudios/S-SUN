// ----------------------------------------------------------------------------------------------
// constants

//
// the begimer
//
var mov_instruction = 1

//
// math
//

var add_instruction=2
var sub_instruction=3
var div_instruction=4
var imul_instruction=5

//
// basic functions
//

var incr_instruction=6
var decr_instruction=7
var jump_instruction=8

//
// inprecindible
//

var ret_instruction=9
var section_instruction=10
var interruption_instruction=11

//
// jumps instructions
//

var jq_instruction=12
var jg_instruction=13
var jng_instruction=14
var jnq_instruction=15

//
// for interact with extern program system parts
//

var extern_ptr_write_instruction=16
var extern_ptr_get_instruction=17

//
// recover and save
//
var extern_ptr_write_instruction=16


//
// other functions
//
var safetynow_for_up = 18
var NULL_PARAM =safetynow_for_up