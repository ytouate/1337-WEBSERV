<?php
header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html>
  <head>
    <title>PHP Test Page</title>
  </head>
  <body>
    <?php
      echo "<h1>PHP Test Page</h1>";
      echo "<p>This page was generated with PHP version " . phpversion() . ".</p>";
    ?>
  </body>
</html>