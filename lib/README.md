The <i>Programming I C Library</i> defines basic data types and functions to simplify C programming for beginners and to enable the step-by-step design approach outlined in the <a href ="/files/prog1script/script.html">script</a>. It contains facilities for testing code, e.g., functions for comparing actual and expected return values. It also contains arrays and lists for elements of various types.

Download and Installation
-------------------------

Download the library with examples from the script:<br/>
<a href="/files/prog1lib-1.2.zip">prog1lib-1.2.zip</a>

Extract the zip file, go to directory prog1lib/lib, make libprog1.a, go to directory prog1lib/script_examples, make celsius_to_fahrenheit (or any other example from the script). In more detail, assuming that you have opened the terminal (on Linux or Mac) or installed MinGW (on Windows, including the @c bin class of msys-wget, msys-zip, msys-unzip), perform these steps:

<code>
cd DirectoryToWhichYouDownloadedTheLibrary<br/>
unzip prog1lib-1.2.zip<br/>
cd prog1lib/lib<br/>
make<br/>
cd ../script_examples<br/>
make celsius_to_fahrenheit<br/>
./celsius_to_fahrenheit<br/>
</code>

Detailed Steps for MinGW on Windows
-----------------------------------

- Download the library and save it in your MinGW home directory:<br/>
<code>
C:\\MinGW\\msys\\1.0\\home\\MyUserName
</code>

- Start the MinGW shell by double-clicking<br/>
<code>
C:\\MinGW\\msys\\1.0\\msys.bat
</code>

- The MinGW shell starts in your MinGW home directory. List its contents:<br/>
<code>
ls -l
</code><br/>
The library zip-file should be in the list.

- Unpack the zip-file to generate the <code>prog1lib</code> directory:<br/>
<code>
unzip prog1lib-1.2.zip
</code><br/>
If <code>unzip</code> is not available, install it via the MinGW setup. (See the exercise slides for more details.)

- Change to the <code>lib</code> directory:<br/>
<code>
cd prog1lib/lib
</code>

- Compile the library:<br/>
<code>
make
</code>

- Change to the script examples directory:<br/>
<code>
cd ../script_examples
</code>

- Compile the example:<br/>
<code>
make celsius_to_fahrenheit
</code>

- Execute the example:<br/>
<code>
./celsius_to_fahrenheit<br/>
</code>
or<br/>
<code>
celsius_to_fahrenheit.exe
</code>
    
- To compile other examples, only perform the last two steps again. They may be combined:<br/>
<code>
make wages && ./wages
</code>



Header Files
------------

Functions for output, input, testing, assertions, etc. contained in @ref base.h.

<a href="/files/prog1lib/files.html">Overview of all files</a>

- base.h
  - basedefs.h
- string.h
- array.h
  - int_array.h
  - double_array.h
  - string_array.h
  - pointer_array.h
  - byte_array.h
- list.h
  - int_list.h
  - double_list.h
  - string_list.h
  - pointer_list.h
- arrays_lists.h

<!--
Array Functions
---------------


array | int array | double array | String array | pointer array | byte array
---- | -------- | ----------- | ----------- | ------------ | ------------
@ref a_create | @ref ia_create | @ref da_create | @ref sa_create | @ref pa_create | @ref ba_create
@ref a_of_buffer | @ref ia_of_buffer | @ref da_of_buffer | @ref sa_of_buffer | @ref pa_of_buffer | @ref ba_of_buffer
- | @ref ia_range | @ref da_range | - | - | -
- | @ref ia_of_string | @ref da_of_string | @ref sa_of_string | - | -
@ref a_fn | @ref ia_fn | @ref da_fn | @ref sa_fn | @ref pa_fn | @ref ba_fn
- | @ref ia_of_da | @ref da_of_ia | - | - | -
@ref a_copy | @ref a_copy | @ref a_copy | @ref a_copy | @ref a_copy | @ref a_copy
@ref a_sub | @ref a_sub | @ref a_sub | @ref a_sub | @ref a_sub | @ref a_sub
@ref a_of_l | @ref a_of_l | @ref a_of_l | @ref a_of_l | @ref a_of_l | @ref a_of_l
@ref a_free | @ref a_free | @ref a_free | @ref sa_free | @ref pa_free | @ref a_free
@ref a_get | @ref ia_get | @ref da_get | @ref sa_get | @ref pa_get | @ref ba_get
@ref a_set | @ref ia_set | @ref da_set | @ref sa_set | @ref pa_set | @ref ba_set
@ref a_length | @ref a_length | @ref a_length | @ref a_length | @ref a_length | @ref a_length
@ref a_element_size | @ref a_element_size | @ref a_element_size | @ref a_element_size | @ref a_element_size | @ref a_element_size
@ref a_print | @ref ia_print | @ref da_print | @ref sa_print | @ref pa_print | @ref ba_print
@ref a_println | @ref ia_println | @ref da_println | @ref sa_println | @ref pa_println | @ref ba_println
@ref a_concat | @ref a_concat | @ref a_concat | @ref a_concat | @ref a_concat | @ref a_concat
- | @ref ia_contains | @ref da_contains | @ref sa_contains | @ref pa_contains | @ref pa_contains
- | @ref ia_fill, @ref ia_fila_from_to [-] | @ref da_fill, @ref da_fila_from_to | @ref sa_fill, @ref sa_fila_from_to | - | -
- | @ref ia_index, @ref ia_index_from | @ref da_index, @ref da_index_from | @ref sa_index, @ref sa_index_from | @ref pa_index, @ref pa_index_from | @ref ba_index, @ref ba_index_from
@ref a_index_fn | @ref ia_index_fn | @ref da_index_fn | @ref sa_index_fn | @ref pa_index_fn | @ref ba_index_fn
@ref a_find | @ref ia_find | @ref da_find | @ref sa_find | @ref pa_find | @ref ba_find
@ref a_reverse | @ref a_reverse | @ref a_reverse | @ref a_reverse | @ref a_reverse | @ref a_reverse
@ref a_shuffle | @ref a_shuffle | @ref a_shuffle | @ref a_shuffle | @ref a_shuffle | @ref a_shuffle
@ref a_sort | @ref ia_sort, @ref ia_sort_dec | @ref da_sort, @ref da_sort_dec | @ref sa_sort, @ref sa_sort_dec | - | -
@ref a_map | @ref ia_map, @ref ia_map_state | @ref da_map, @ref da_map_state | @ref sa_map, @ref sa_map_state | @ref pa_map | @ref ba_map, @ref ba_map_state
@ref a_each | @ref ia_each, @ref ia_each_state | @ref da_each, @ref da_each_state | @ref sa_each, @ref sa_each_state | @ref pa_each | @ref ba_each, @ref ba_each_state
@ref a_foldl | @ref ia_foldl | @ref da_foldl | @ref sa_foldl | @ref pa_foldl | @ref ba_foldl
@ref a_foldr | @ref ia_foldr | @ref da_foldr | @ref sa_foldr | @ref pa_foldr | @ref ba_foldr
@ref a_filter | @ref ia_filter, @ref ia_filter_state | @ref da_filter, @ref da_filter_state | @ref sa_filter, @ref sa_filter_state | @ref pa_filter | @ref ba_filter, @ref ba_filter_state
@ref a_choose | @ref ia_choose, @ref ia_choose_state | @ref da_choose, @ref da_choose_state | @ref sa_choose, @ref sa_choose_state | @ref pa_choose | @ref ba_choose, @ref ba_choose_state
@ref a_exists | @ref ia_exists, @ref ia_exists_state | @ref da_exists, @ref da_exists_state | @ref sa_exists, @ref sa_exists_state | @ref pa_exists | @ref ba_exists, @ref ba_exists_state
@ref a_forall | @ref ia_forall, @ref ia_forall_state | @ref da_forall, @ref da_forall_state | @ref sa_forall, @ref sa_forall_state | @ref pa_forall | @ref ia_forall, @ref ia_forall_state
@ref a_test_equal | @ref ia_test_equal | @ref da_test_within | @ref sa_test_equal | @ref pa_test_equal | @ref ba_test_equal



List Functions
--------------


list | int list | double list | String list | pointer list
---- | -------- | ----------- | ----------- | ------------
@ref l_create | @ref il_create | @ref dl_create | @ref sl_create | @ref pl_create
@ref l_of_buffer | @ref il_of_buffer | @ref dl_of_buffer | @ref sl_of_buffer | @ref pl_of_buffer
- | @ref il_range | @ref dl_range | - | -
- | @ref il_of_string | @ref dl_of_string | @ref sl_of_string | -
@ref l_fn | @ref il_fn | @ref dl_fn | @ref sl_fn | @ref pl_fn
- | @ref il_of_dl | @ref dl_of_il | - | -
@ref l_copy | @ref l_copy | @ref l_copy | @ref l_copy | @ref l_copy
@ref l_sub | @ref l_sub | @ref l_sub | @ref l_sub | @ref l_sub
@ref l_of_a | @ref l_of_a | @ref l_of_a | @ref l_of_a | @ref l_of_a
@ref l_free | @ref l_free | @ref l_free | @ref sl_free | @ref pl_free, @ref pl_free_with_destructor
@ref l_get | @ref il_get | @ref dl_get | @ref sl_get | @ref pl_get
@ref l_set | @ref il_set | @ref dl_set | @ref sl_set | @ref pl_set
@ref l_iterator | @ref l_iterator | @ref l_iterator | @ref l_iterator | @ref l_iterator
@ref l_has_next | @ref l_has_next | @ref l_has_next | @ref l_has_next | @ref l_has_next
@ref l_next | @ref il_next | @ref dl_next | @ref sl_next | @ref pl_next
@ref l_append | @ref il_append | @ref dl_append | @ref sl_append | @ref pl_append
@ref l_prepend | @ref il_prepend | @ref dl_prepend | @ref sl_prepend | @ref pl_prepend
@ref l_length | @ref l_length | @ref l_length | @ref l_length | @ref l_length
@ref l_element_size | @ref l_element_size | @ref l_element_size | @ref l_element_size | @ref l_element_size
@ref l_print | @ref il_print | @ref dl_print | @ref sl_print | @ref pl_print
@ref l_println | @ref il_println | @ref dl_println | @ref sl_println | @ref pl_println
@ref l_concat | @ref l_concat | @ref l_concat | @ref l_concat | @ref l_concat
- | @ref il_contains | @ref dl_contains | @ref sl_contains | @ref pl_contains
- | @ref il_fill, @ref il_fill_from_to [-] | @ref dl_fill, @ref dl_fill_from_to | @ref sl_fill, @ref sl_fill_from_to | -
- | @ref il_index, @ref il_index_from | @ref dl_index, @ref dl_index_from | @ref sl_index, @ref sl_index_from | @ref pl_index, @ref pl_index_from
@ref l_index_fn | @ref il_index_fn | @ref dl_index_fn | @ref sl_index_fn | @ref pl_index_fn
@ref l_find | @ref il_find | @ref dl_find | @ref sl_find | @ref pl_find
@ref l_reverse | @ref l_reverse | @ref l_reverse | @ref l_reverse | @ref l_reverse
@ref l_shuffle | @ref l_shuffle | @ref l_shuffle | @ref l_shuffle | @ref l_shuffle
@ref l_sort | @ref il_sort, @ref il_sort_dec | @ref dl_sort, @ref dl_sort_dec | @ref sl_sort, @ref sl_sort_dec | -
@ref l_insert | @ref il_insert | @ref dl_insert | @ref sl_insert | @ref pl_insert
@ref l_remove | @ref il_remove | @ref dl_remove | @ref sl_remove | @ref pl_remove
@ref l_map | @ref il_map, @ref il_map_state | @ref dl_map, @ref dl_map_state | @ref sl_map, @ref sl_map_state | @ref pl_map
@ref l_each | @ref il_each, @ref il_each_state | @ref dl_each, @ref dl_each_state | @ref sl_each, @ref sl_each_state | @ref pl_each
@ref l_foldl | @ref il_foldl | @ref dl_foldl | @ref sl_foldl | @ref pl_foldl
@ref l_foldr | @ref il_foldr | @ref dl_foldr | @ref sl_foldr | @ref pl_foldr
@ref l_filter | @ref il_filter, @ref il_filter_state | @ref dl_filter, @ref dl_filter_state | @ref sl_filter, @ref sl_filter_state | @ref pl_filter
@ref l_choose | @ref il_choose, @ref il_choose_state | @ref dl_choose, @ref dl_choose_state | @ref sl_choose, @ref sl_choose_state | @ref pl_choose
@ref l_exists | @ref il_exists, @ref il_exists_state | @ref dl_exists, @ref dl_exists_state | @ref sl_exists, @ref sl_exists_state | @ref pl_exists
@ref l_forall | @ref il_forall, @ref il_forall_state | @ref dl_forall, @ref dl_forall_state | @ref sl_forall, @ref sl_forall_state | @ref pl_forall
@ref l_test_equal | @ref il_test_equal | @ref dl_test_within | @ref sl_test_equal | @ref pl_test_equal

-->
