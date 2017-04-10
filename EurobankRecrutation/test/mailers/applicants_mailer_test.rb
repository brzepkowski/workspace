require 'test_helper'

class ApplicantsMailerTest < ActionMailer::TestCase
  test "invitation" do
    mail = ApplicantsMailer.invitation
    assert_equal "Invitation", mail.subject
    assert_equal ["to@example.org"], mail.to
    assert_equal ["from@example.com"], mail.from
    assert_match "Hi", mail.body.encoded
  end

  test "rejection" do
    mail = ApplicantsMailer.rejection
    assert_equal "Rejection", mail.subject
    assert_equal ["to@example.org"], mail.to
    assert_equal ["from@example.com"], mail.from
    assert_match "Hi", mail.body.encoded
  end

end
