# Preview all emails at http://localhost:3000/rails/mailers/applicants_mailer
class ApplicantsMailerPreview < ActionMailer::Preview

  # Preview this email at http://localhost:3000/rails/mailers/applicants_mailer/invitation
  def invitation
    ApplicantsMailer.invitation
  end

  # Preview this email at http://localhost:3000/rails/mailers/applicants_mailer/rejection
  def rejection
    ApplicantsMailer.rejection
  end

end
