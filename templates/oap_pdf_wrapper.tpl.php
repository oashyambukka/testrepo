<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta charset="UTF-8"/>
  <link href="form.css" rel="stylesheet" type="text/css" />
  <title><?php print $formtitle; ?></title>
</head>

<body>
<div id="wrapper">
  <div class="header">
    <div class="formTitle"><?php print $formtitle; ?></div>
    <div id="userinfo">
      <p class="userdata">NAME: <?php print $name; ?></p>
      <p class="userdata">ID: <?php print $rmaxid; ?></p>
    </div>
  </div>

  <div id="content">
<?php print $body; ?>
  </div>
</div>
</body>
</html>
