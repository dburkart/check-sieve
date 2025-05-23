import unittest
import checksieve

class ExamplesTest(unittest.TestCase):

    def test_example_01(self):
        sieve = '''
        require [ "processcalendar" ];

        processcalendar :addresses [ "me@example.com", "alsome@example.com" ]
                        :calendarid "1ea6d86b-6c7f-48a2-bed3-2a4c40ec281a";        
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_02(self):
        sieve = '''
        require [ "processcalendar" ];

        if allof (address ["from", "sender"] "airline@example.com",
                  header :contains "subject" "itinerary") {
           processcalendar :allowpublic;
        }        
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_03(self):
        sieve = '''
        require [ "processcalendar", "variables", "editheader" ];

        set "processcal_outcome" "no_action";
        set "processcal_reason" "";

        processcalendar :outcome "processcal_outcome"
                        :reason "processcal_reason";

        if not string :is "${processcal_outcome}" ["added", "updated"] {
           addheader "X-ProcessCal-Outcome" "${processcal_outcome}";
           addheader "X-ProcessCal-Reason" "${processcal_reason}";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))