import unittest
import checksieve

class CrossRequiresTest(unittest.TestCase):

    def test_extlists_organizers_no(self):
        sieve = '''
        require [ "processcalendar" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :organizers "foo@example.com";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_extlists_organizers_yes(self):
        sieve = '''
        require [ "processcalendar", "extlists" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :organizers "foo@example.com";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_variables_outcome_no(self):
        sieve = '''
        require [ "processcalendar" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :outcome "process_outcome";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_variables_outcome_yes(self):
        sieve = '''
        require [ "processcalendar", "variables" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :outcome "process_outcome";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_variables_reason_no(self):
        sieve = '''
        require [ "processcalendar" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :reason "process_reason";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_variables_reason_yes(self):
        sieve = '''
        require [ "processcalendar", "variables" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a"
                        :reason "process_reason";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
