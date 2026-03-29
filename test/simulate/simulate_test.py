import unittest
from . import util


class TestSimulateAddressDomain(util.DiffTestCase):

    def test_address_domain_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('address_domain.sieve', 'basic.eml'), 'address_domain_basic.out'))

    def test_address_domain_no_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('address_domain.sieve', 'spam.eml'), 'address_domain_spam.out'))


class TestSimulateAddressLocalpart(util.DiffTestCase):

    def test_address_localpart_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('address_localpart.sieve', 'basic.eml'), 'address_localpart_basic.out'))

    def test_address_localpart_no_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('address_localpart.sieve', 'spam.eml'), 'address_localpart_spam.out'))


class TestSimulateAllofAnyof(util.DiffTestCase):

    def test_allof_anyof_all_present(self):
        self.assertNoDiff(util.diff(util.run_simulate('allof_anyof.sieve', 'basic.eml'), 'allof_anyof_basic.out'))

    def test_allof_anyof_partial(self):
        self.assertNoDiff(util.diff(util.run_simulate('allof_anyof.sieve', 'spam.eml'), 'allof_anyof_spam.out'))


class TestSimulateBoolean(util.DiffTestCase):

    def test_boolean_true_false(self):
        self.assertNoDiff(util.diff(util.run_simulate('boolean.sieve', 'basic.eml'), 'boolean_basic.out'))


class TestSimulateElseBranch(util.DiffTestCase):

    def test_else_branch(self):
        self.assertNoDiff(util.diff(util.run_simulate('else_branch.sieve', 'basic.eml'), 'else_branch_basic.out'))


class TestSimulateExistsStop(util.DiffTestCase):

    def test_exists_match_and_stop(self):
        self.assertNoDiff(util.diff(util.run_simulate('exists_stop.sieve', 'basic.eml'), 'exists_stop_basic.out'))

    def test_exists_no_match_implicit_keep(self):
        self.assertNoDiff(util.diff(util.run_simulate('exists_stop.sieve', 'spam.eml'), 'exists_stop_spam.out'))


class TestSimulateHeaderContains(util.DiffTestCase):

    def test_header_contains_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('header_contains.sieve', 'basic.eml'), 'header_contains_basic.out'))

    def test_header_contains_no_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('header_contains.sieve', 'spam.eml'), 'header_contains_spam.out'))


class TestSimulateHeaderMatches(util.DiffTestCase):

    def test_header_matches_glob(self):
        self.assertNoDiff(util.diff(util.run_simulate('header_matches.sieve', 'basic.eml'), 'header_matches_basic.out'))

    def test_header_matches_glob_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('header_matches.sieve', 'spam.eml'), 'header_matches_spam.out'))


class TestSimulateImplicitKeep(util.DiffTestCase):

    def test_implicit_keep(self):
        self.assertNoDiff(util.diff(util.run_simulate('implicit_keep.sieve', 'basic.eml'), 'implicit_keep_basic.out'))


class TestSimulateRedirectDiscard(util.DiffTestCase):

    def test_redirect_on_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('redirect_discard.sieve', 'basic.eml'), 'redirect_discard_basic.out'))

    def test_discard_on_no_match(self):
        self.assertNoDiff(util.diff(util.run_simulate('redirect_discard.sieve', 'spam.eml'), 'redirect_discard_spam.out'))


class TestSimulateSize(util.DiffTestCase):

    def test_size_under_and_over(self):
        self.assertNoDiff(util.diff(util.run_simulate('size_over.sieve', 'basic.eml'), 'size_over_basic.out'))

    def test_size_small_message(self):
        self.assertNoDiff(util.diff(util.run_simulate('size_over.sieve', 'spam.eml'), 'size_over_spam.out'))


if __name__ == '__main__':
    unittest.main()
