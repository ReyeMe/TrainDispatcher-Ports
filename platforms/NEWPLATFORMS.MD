# How 2 add a new platform

In order to make the game actually run, and not crash first thing before you even try, you need to set up the following:

## p_alloc and p_free

    p_alloc = your_alloc_here;
    p_free = your_free_here;
    p_panic = your_error_handler_here;
