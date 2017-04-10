class ApplicantsMailer < ActionMailer::Base
	default from: "rekrutacja_eurobank_test@wp.pl"

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.applicants_mailer.invitation.subject
  #	

  def invite(application)
		@application = application
		
    @greeting = "Eurobank rekrutacja - zaproszenie na rozmowę kwalifikacyjną"

    mail to: @application.email
  end

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.applicants_mailer.rejection.subject
  #
  def rejection
    @greeting = "Eurobank rekrutacja"

    mail to: "to@example.org"
  end
end
