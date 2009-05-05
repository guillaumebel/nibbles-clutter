#include <config.h>
#include <stdlib.h>
#include <check.h>

#include <gtk/gtk.h>

#include <libgames-support/games-clock.h>

/*  Fixtures used to prepare and cleanup for testing. */
static void
fx_setup_gtk ()
{
  int argc = 0;

  gtk_init (&argc, NULL);

}

static void 
fx_teardown_gtk ()
{
  /*  nothing to do.. */ 
}


/* TESTCASE 1: Simple test case for games-clock.c */
START_TEST (test_games_clock_new)
{
  GtkWidget *clock;

  /* test clock initialization. */
  clock = games_clock_new ();
  fail_if (!GTK_IS_WIDGET (clock), 
           "games_clock_new() failed.");

  /*  test starting of clock */
  games_clock_start (GAMES_CLOCK (clock));
  fail_unless (clock && GAMES_IS_CLOCK (clock));

  /*  test stopping of clock */
  games_clock_stop (GAMES_CLOCK (clock));
  fail_unless (clock && GAMES_IS_CLOCK (clock));
}
END_TEST


/* TESTCASE 2: Setting and getting time with games-clock.c */
START_TEST (test_games_clock_set)
{
  GtkWidget *clock;
  time_t end;

  clock = games_clock_new ();
  fail_if (!GTK_IS_WIDGET (clock), 
           "games_clock_new() failed.");

  games_clock_set_seconds (GAMES_CLOCK (clock), 10);
  games_clock_add_seconds (GAMES_CLOCK (clock), 10);
  end = games_clock_get_seconds (GAMES_CLOCK (clock));

  fail_unless (end == 20, 
               "Expected current time to be 20, "
	       "but the games-clock returned %d instead", end);
  
}
END_TEST


/* Create test suite for libgames-support  */
static Suite *
libgames_support_suite (void)
{
  Suite *s = suite_create ("libgames-support");
  
  TCase *tc1 = tcase_create ("games-clock-new");
  tcase_add_test (tc1, test_games_clock_new);
  tcase_add_checked_fixture (tc1, fx_setup_gtk, fx_teardown_gtk);
  suite_add_tcase (s, tc1);

  TCase *tc2 = tcase_create ("games-clock-set");
  tcase_add_test (tc2, test_games_clock_set);
  tcase_add_checked_fixture (tc2, fx_setup_gtk, fx_teardown_gtk);
  tcase_set_timeout(tc2, 0);
  suite_add_tcase (s, tc2);

  
  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = libgames_support_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
} 
