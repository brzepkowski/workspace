  use HTTP::Daemon;
  use HTTP::Status;  
  use IO::File;

  my $d = HTTP::Daemon->new(
           LocalAddr => 'localhost',
           LocalPort => 4321,
       )|| die;

  $DIR = '/home/bartas/workspace/Network_Technologies_5';
  
  print "Please contact me at: <URL:", $d->url, ">\n";


  while (my $c = $d->accept) {
      while (my $r = $c->get_request) {
          if ($r->method eq 'GET') {

		if($r->uri eq "/") {
			$file_s = "/index.html";
		}
		else {
			$file_s = $r->uri;
		}

		$c->send_file_response($DIR.$file_s);

          }
          else {
              $c->send_error(RC_FORBIDDEN);
          }

      }
      $c->close;
      undef($c);
  }
